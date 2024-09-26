#pragma once
#include "glpointer.hpp"
#include <glm/detail/qualifier.hpp>
#include <vector>

namespace toto {

template <GLsizei N = 1>
class GLBuffer : public GLPointerArray<
                     [](GLsizei n, GLuint* buffers) { glGenBuffers(n, buffers); },
                     [](GLsizei n, GLuint* buffers) { glDeleteBuffers(n, buffers); }, N> {
public:
    GLBuffer() = default;
    ~GLBuffer() = default;
    GLBuffer(GLBuffer&&) = default;
    GLBuffer& operator=(GLBuffer&&) = default;

    inline static void bind(GLenum target, const GLBuffer& buffer) { glBindBuffer(target, buffer.handle()); }
    inline static void unbind(GLenum target) { glBindBuffer(target, 0); }

    inline void bind(GLenum target) const { bind(target, *this); }

    template <typename T>
    void data(GLenum target, const std::vector<T>& data, GLenum usage) const {
        bind(target, *this);
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }
};

template <GLsizei N = 1>
class GLVertexArray : public GLPointerArray<
                          [](GLsizei n, GLuint* arrays) { glGenVertexArrays(n, arrays); },
                          [](GLsizei n, GLuint* arrays) { glDeleteVertexArrays(n, arrays); }, N> {
public:
    GLVertexArray() = default;
    ~GLVertexArray() = default;
    GLVertexArray(GLVertexArray&&) = default;
    GLVertexArray& operator=(GLVertexArray&&) = default;

    inline static void bind(const GLVertexArray& array) { glBindVertexArray(array.handle()); }
    inline static void unbind() { glBindVertexArray(0); }

    inline void bind() const { bind(*this); }

    /**
     * @brief Set the Vertex Attrib Pointer
     *
     * @tparam VAOType The struct that you are using
     * @tparam SubType The number type for the attribute
     * @tparam SIZE The size of the attribute
     * @tparam OFFSET The offset of the attribute
     * @param index
     */
    template <typename VAOType, typename SubType, size_t SIZE, size_t OFFSET>
    void setVertexAttrib(GLuint index) const {
        bind();
        glEnableVertexAttribArray(index);
        if constexpr (std::is_same_v<SubType, double>) {
            glVertexAttribPointer(index, SIZE, GL_DOUBLE, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, float>) {
            glVertexAttribPointer(index, SIZE, GL_FLOAT, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, int>) {
            glVertexAttribIPointer(index, SIZE, GL_INT, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, unsigned int>) {
            glVertexAttribIPointer(index, SIZE, GL_UNSIGNED_INT, sizeof(VAOType), (void*)OFFSET);
        } else {
            static_assert(false, "Invalid SubType");
        }
    }

    /**
     * @brief Set the Vertex Attrib Pointer
     *
     * @tparam VAOType The struct that you are using
     * @tparam VecType The glm::vec type for the attribute. Only works for glm::vec<N, float, defaultp> (aka glm::vecN)
     * @tparam OFFSET The offset of the attribute
     * @param index
     */
    template <typename VAOType, typename VecType, size_t OFFSET>
    void setVertexAttrib(GLuint index) const {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, VecType::length(), GL_FLOAT, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
    }
};

} // namespace toto
