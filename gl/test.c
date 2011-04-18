#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    /* X11 stuff */
    Display* x_display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(x_display);

    XSetWindowAttributes window_attributes;
    window_attributes.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;

    Window window = XCreateWindow(x_display, root, 0, 0, 800, 600, 0,
                                  CopyFromParent, InputOutput, CopyFromParent,
                                  CWEventMask, &window_attributes);


    XMapWindow(x_display, window);

    /* EGL */
    EGLDisplay egl_display = eglGetDisplay((EGLNativeDisplayType)x_display);
    if (!eglInitialize(egl_display, NULL, NULL)) {
        g_error("Failed to initialize EGL display.");
        return 1;
    }

    EGLint egl_attributes[] = {
        EGL_BUFFER_SIZE, 16,
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig egl_config;
    EGLint number_of_configs;
    eglChooseConfig(egl_display, egl_attributes, &egl_config, 1, &number_of_configs);
    if (number_of_configs != 1) {
        g_error("We need 1 configuration!");
        return 1;
    }

    EGLSurface egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)window, NULL);
    if (egl_surface == EGL_NO_SURFACE) {
        g_error("No surface...");
        return 1;
    }

    EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attributes);
    if (egl_context == EGL_NO_CONTEXT) {
        g_error("No context...");
        return 1;
    }

    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    /* Set the viewport */
    glViewport(0, 0, 800, 600);

    /* Paint the background */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(egl_display, egl_surface);

    /* Prepare the shaders and the program */
    const char* vertex_shader_src = \
        "attribute vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";

    const char* fragment_shader_src = \
        "precision mediump float;"
        "void main()"
        "{"
        "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";

    GLint success;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (!vertex_shader) {
        g_error("Failed creating vertex shader.");
        return 1;
    }
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        g_error("Failed to compile vertext shader!");
        return 1;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        g_error("Failed creating fragment shader.");
        return 1;
    }
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        g_error("Failed to compile fragment shader!");
        return 1;
    }

    GLuint program_object = glCreateProgram();

    if (!program_object) {
        g_error("No program!");
        return 1;
    }

    /* Attach shaders */
    glAttachShader(program_object, vertex_shader);
    glAttachShader(program_object, fragment_shader);

    /* Bind attribute */
    glBindAttribLocation(program_object, 0, "position");

    /* Link */
    glLinkProgram(program_object);

    glGetProgramiv(program_object, GL_LINK_STATUS, &success);
    if (!success) {
        g_error("Linking the program failed.");
        return 1;
    }

    glUseProgram(program_object);

    /* Draw the triangle */
    GLfloat vertices[] = { 0.0f, 0.5f, -0.5f,
                           -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f };
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(egl_display, egl_surface);

    /* Main loop */
    GMainLoop* loop = g_main_loop_new(NULL, TRUE);
    g_main_loop_run(loop);

    return 0;
}

