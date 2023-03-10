#pragma once
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Helper/stb_image/stb_image.h>
#include <Helper/log/log.h>
#include <Helper/glm/glm.hpp>
#include <Helper/glm/gtc/matrix_transform.hpp>
#include <Helper/glm/gtc/type_ptr.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>
#include <type_traits>
#include <iostream>
#include <functional>
#ifndef STRICT_
#    define STRICT_ true
#endif
struct ScopeObject
{
    std::function<void()> cons;
    std::function<void()> decons;
    ScopeObject(std::function<void()> cons_, std::function<void()> decons_)
        : cons{std::move(cons_)}
        , decons{std::move(decons_)}
    {
        if (cons != nullptr)
            cons();
    }
    ~ScopeObject()
    {
        if (decons != nullptr)
            decons();
    }
};

inline auto readFile(const std::string &f) -> std::string
{
    try
    {
        //  system("dir");
        std::ifstream fin;
        fin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fin.open(f);
        std::stringstream ss;
        ss << fin.rdbuf();
        auto ret = ss.str();
        return ret;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        GLLogger()->error("readFile Error:[file:{},err:{}] at {}:{}", f, e.what(), __FILE__, __LINE__);
    }
    catch (...)
    {
        throw; // this should not be reached
    }
    return "";
}

inline auto writeFile(const std::string &fName, const std::string &data)
{
    std::ofstream out(fName);
    out << data;
    out.close();
    return out.good();
}

using AttributeInfo = struct
{
    std::string name;
    GLenum      type;
    GLint       position;
    GLint       size;
};

// use after program link
inline auto getAttributes(GLint program)
{
    std::map<std::string, AttributeInfo> ret;
    int                                  count = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    constexpr int name_length = 256;
    char          cache_name[name_length];
    GLint         used_cache_name_length = 0;
    for (int i = 0; i < count; ++i)
    {
        AttributeInfo info;
        glGetActiveAttrib(program, GLint(i), name_length, &used_cache_name_length, &info.size, &info.type, cache_name);
        info.name = std::string(cache_name, cache_name + used_cache_name_length);
        info.position = glGetAttribLocation(program, info.name.c_str());
        ret[info.name] = info;
    }
    return ret;
}

// \ret
// ret==0: success
// ret&1: checkAttributesExistence failed
// ret&2: checkPositionCorrectness failed
template<bool checkAttributesExistence = STRICT_, bool checkPositionCorrectness = STRICT_>
inline int bindAttributesLocations(GLint program, std::vector<std::pair<std::string, GLint>> const &lcos)
{
    int ret = 0;
    if constexpr (checkAttributesExistence)
        for (auto &[name, _] : lcos)
            if (-1 == glGetAttribLocation(program, name.c_str()))
            {
                ret += 1;
                break;
            }
    std::set<int> s;
    for (auto &[name, pos] : lcos)
    {
        glBindAttribLocation(program, pos, name.c_str());
        s.insert(pos);
    }
    // simply check size
    if constexpr (checkPositionCorrectness)
        if (int(s.size()) < int(lcos.size()))
            ret += 2;
    glLinkProgram(program);
    return ret;
}

template<int dim, typename T, glm::qualifier Q>
inline auto to_string(typename glm::vec<dim, T, Q> const &v)
{
    std::string ret;
    if constexpr (dim > 0)
        ret += std::to_string(v.x) + ' ';
    if constexpr (dim > 1)
        ret += std::to_string(v.y) + ' ';
    if constexpr (dim > 2)
        ret += std::to_string(v.z) + ' ';
    if constexpr (dim > 3)
        ret += std::to_string(v.w) + ' ';
    return ret;
}

// from left to right
// if these transform matrix let
template<typename... restMAT>
inline int ProjectionOutOfRange(glm::vec4 c)
{
    return c.x > 1 || c.x < -1 || c.y > 1 || c.y < -1 || c.z > 1 || c.z < -1;
}
template<typename... restMAT>
inline int ProjectionOutOfRange(glm::vec4 v, restMAT... rest)
{
    auto vv = (... * rest) * v;
    return ProjectionOutOfRange(vv);
}

// no postfix!!!
inline auto getSrcFileNameOnlyName(std::string const &fullName)
{
    auto begin = fullName.find_last_of("/\\"), end = fullName.find_last_of('.');
    if (begin == std::string::npos)
        begin = 0;
    auto ret = fullName.substr(begin,end);
    return ret;
}

template<typename T, typename T1 = T, typename T2 = T>
inline auto clamp(T val, T1 minVal, T2 maxVal) -> T
{
    static_assert(std::is_convertible_v<T1, T> && std::is_convertible_v<T2, T>, "T1 or T2 is not convertable to T");
    if (val < minVal)
        return minVal;
    if (val > maxVal)
        return maxVal;
    return val;
}

template<typename UNIT, typename TO>
std::string toRGB(UNIT *data, int pixelLength)
{
    std::string ret;
    ret.resize(3 * pixelLength * sizeof(TO));
    for (int i = 0; i < pixelLength; ++i)
    {
        void *p = ret.data() + sizeof(TO) * i * 3;
        TO   *pp = reinterpret_cast<TO *>(p);
        pp[0] = data[i * 3];
        pp[1] = data[i * 3 + 1];
        pp[2] = data[i * 3 + 2];
    }
    return ret;
}
