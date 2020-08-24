#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <errno.h>
#include <fcntl.h>
#include <gbm.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    int drm_fd_;
    if((drm_fd_ = open("/dev/dri/renderD128", O_RDWR)) < 0) {
      fprintf(stderr, "can't open device: %s\n", strerror(errno));
      return 1;
    }

    struct gbm_device *gbm_;
    if(!(gbm_ = gbm_create_device(drm_fd_))) {
      fprintf(stderr, "can't create gbm device: %s\n", strerror(errno));
      return 1;
    }

    struct gbm_surface *gbm_surf;
    if(!(gbm_surf = gbm_surface_create(gbm_, 800, 600,  GBM_BO_FORMAT_XRGB8888, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING))) {
      fprintf(stderr, "can't create gbm surface: %s\n", strerror(errno));
      return 1;
    }

    EGLDisplay egl_display_ = eglGetPlatformDisplay(EGL_PLATFORM_GBM_KHR, gbm_, NULL);
    if(EGL_NO_DISPLAY == egl_display_) {
        fprintf(stderr, "EGL_NO_DISPLAY\n");
        return 1;
    } else {
        fprintf(stderr, "EGL display!\n");
    }

    return 0;
}
