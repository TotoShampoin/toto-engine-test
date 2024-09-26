#pragma once
#include "toto-engine/import-gl.hpp"
#include <vector>

namespace toto {

template <void (*construct)(GLsizei, GLuint*), void (*destruct)(GLsizei, GLuint*)>
class GLPointer {
public:
    GLPointer() { construct(1, &_handle); }
    ~GLPointer() { destruct(1, &_handle); }
    GLPointer(GLPointer&& other) {
        _handle = other._handle;
        other._handle = 0;
    }
    GLPointer& operator=(GLPointer&& other) {
        if (this == &other)
            return *this;
        destruct(1, &_handle);
        _handle = other._handle;
        other._handle = 0;
        return *this;
    }
    GLPointer(const GLPointer&) = delete;
    GLPointer& operator=(const GLPointer&) = delete;

    inline GLuint handle() { return _handle; }
    inline GLuint operator*() { return _handle; }

    operator GLuint() { return _handle; }

private:
    GLuint _handle {0};
};

class GLBuffer : public GLPointer<
                     [](GLsizei n, GLuint* buffers) { glGenBuffers(n, buffers); },
                     [](GLsizei n, GLuint* buffers) { glDeleteBuffers(n, buffers); }> {
public:
    GLBuffer() = default;
    ~GLBuffer() = default;
    GLBuffer(GLBuffer&&) = default;
    GLBuffer& operator=(GLBuffer&&) = default;

    inline static void bind(GLenum target, GLBuffer& buffer) { glBindBuffer(target, buffer); }
    inline static void unbind(GLenum target) { glBindBuffer(target, 0); }

    inline void bind(GLenum target) { bind(target, *this); }

    template <typename T>
    void data(GLenum target, const std::vector<T>& data, GLenum usage) {
        bind(target, *this);
        glBufferData(target, data.size() * sizeof(T), data.data(), usage);
    }
};

class GLVertexArray : public GLPointer<
                          [](GLsizei n, GLuint* arrays) { glGenVertexArrays(n, arrays); },
                          [](GLsizei n, GLuint* arrays) { glDeleteVertexArrays(n, arrays); }> {
public:
    GLVertexArray() = default;
    ~GLVertexArray() = default;
    GLVertexArray(GLVertexArray&&) = default;
    GLVertexArray& operator=(GLVertexArray&&) = default;

    inline static void bind(GLVertexArray& array) { glBindVertexArray(array); }
    inline static void unbind() { glBindVertexArray(0); }

    inline void bind() { bind(*this); }

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
    void setVertexAttrib(GLuint index) {
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
     * @tparam VecType The glm::vec type for the attribute. MUST BE A FLOAT VECTOR
     * @tparam OFFSET The offset of the attribute
     * @param index
     */
    template <typename VAOType, typename VecType, size_t OFFSET>
    void setVertexAttrib(GLuint index) {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, VecType::length(), GL_FLOAT, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
    }
};

} // namespace toto
