#include "gfx_context.h"

namespace sraphy
{
    namespace
    {
#if defined(SR_DEBUG)
        static void DebugCallback(GLenum p_Source, GLenum p_Type, GLuint /*id*/, GLenum p_Severity, GLsizei /*length*/, GLchar const* p_Message, void const* /*user_param*/)
        {
            auto const message_source = [p_Source]
            {
                switch (p_Source)
                {
                case GL_DEBUG_SOURCE_API:             return "OpenGL API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
                case GL_DEBUG_SOURCE_APPLICATION:     return "User Created App";
                default:                              return "Unknown";
                }
            }();

            auto const message_type = [p_Type]
            {
                switch (p_Type)
                {
                case GL_DEBUG_TYPE_ERROR:				return "Error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "Deprecated Behaviour";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "Undefined Behaviour";
                case GL_DEBUG_TYPE_PORTABILITY:			return "Portability Issue";
                case GL_DEBUG_TYPE_PERFORMANCE:			return "Performance Issue";
                case GL_DEBUG_TYPE_MARKER:				return "Marker";
                case GL_DEBUG_TYPE_PUSH_GROUP:			return "Push Group";
                case GL_DEBUG_TYPE_POP_GROUP:			return "Pop Group";
                default:                                return "Unknown";
                }
            }();

            LOG_TRACE("OpenGL Context\n");
            switch (p_Severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_ERROR("{0} on {1}, {2}", message_type, message_source, p_Message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_WARN("{0} on {1}, {2}", message_type, message_source, p_Message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_INFO("Notification: {0}, Source: {1}, {2}", message_type, message_source, p_Message);
                break;
            }
        }
#endif /* SR_DEBUG */
    }

    void InitializeGfxContext(GLFWwindow* p_WinHandle)
    {
        int32_t result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        if (!result)
        {
            LOG_ERROR("GLAD was not initialized!");
            return;
        }
#if defined(SR_DEBUG)
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
    }
} // namespace sraphy
