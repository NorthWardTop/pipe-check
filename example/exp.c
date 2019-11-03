#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


int status = 1;   //停止标志位，1开始，0停止

/* 监听线程 当启动摄像头后从键盘输入q，结束程序 */
void * listen(void * arg) 
{
    while(1) {
        char buf[20];
        scanf("%s", buf);
        if(strcmp("q", buf) == 0) {
            status = 0;
            break;
        }
        usleep(10);
    }
}

/* 屏幕初始化和销毁函数 */
char * fb_init(char * devname, int * fd, int * len);
void fb_destory(int fd, char * screen_bbf, int screenlen); 

/* 
    函数功能：进行图像转换，将uvc输出的yuyv格式图像转换成RGB32格式的图像
    返回值：无
    参数： 
        yuv yuyv格式图像存储地址
        buf RGB32格式图像存储地址
        length 图像的大小（单位：字节）
*/
void process_image(unsigned char * yuv, unsigned char *buf, int length);
/*
    函数功能：向屏幕输出图像
    返回值：无
    参数 ： screen_bbf  内存映射后屏幕在程序中的地址
                    buf                 RGB32格式的数据地址
                    width               图像的宽度
                    height          图像的高度
*/
void show_image(char * screen_bbf, char *buf, int width, int height);


/* 保存摄像头内存映射后的内存地址和数据长度 */
struct buffer {
    char * start;
    unsigned int length;
};

int width,height;

int main(int argc, char ** argv)
{
    /* 摄像头采集的是YUYV格式的图像， 
        屏幕显示需要RGB32格式的图像，
        而且屏幕大小和摄像头的视野大小也不一样
        这里申请一块内存作为缓冲区，存储格式转换后的数据
     */
    unsigned char * bbf = (unsigned char *)malloc(800*480*4);  
    printf(" bbf address : %p\n", bbf);
    /**************** 设置屏幕 ****************/
    int fb_fd, screenlen;
    char * screen_bbf = fb_init("/dev/fb0", &fb_fd, &screenlen);

    /*************** 开始设置摄像头 ********************/
    /* 打开摄像头设备 */
    int cam_fd = open("/dev/video15", O_RDWR);
    if (cam_fd == -1) {
        printf("error : %s\n", strerror(errno));
        return -1;
    }
    /* 得到描述摄像头信息的结构体 */
    struct v4l2_capability cap;
    int rel = ioctl(cam_fd, VIDIOC_QUERYCAP, &cap);
    if ( rel == -1) {
        printf("error : %s\n", strerror(errno));
        goto ERROR;
    }
    /* 判断改设备支不支持捕获图像和流输出功能 */
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
        printf("it's camer!\n");
    else {
        printf("it's not a camer!\n");
        goto ERROR;
    }
    if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
        printf("it's stream device!\n");
    else {
        printf("it's not a stream device!\n");
        goto ERROR;
    }

    printf("Driver Name : %s\n\
    Card Name : %s\nBus info : %s\n\
    Driver Version : %u.%u.%u\n ",\
    cap.driver, cap.card, cap.bus_info,\
     (cap.version>>16)&0xff, (cap.version>>8)&0xff, (cap.version)&0xff);

    /* 得到摄像头采集图像的格式信息 */
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rel = ioctl(cam_fd, VIDIOC_G_FMT, &fmt);
    if (rel == -1) {
        printf("get fmt failed!\n");
        goto ERROR;
    }

    width = fmt.fmt.pix.width;
    height = fmt.fmt.pix.height;

    printf("width : %d  height : %d\n\
    pixelformat : %d\n\
    field : %d\n\
    bytesperline : %d\n\
    sizeimage : %d\n\
    colorspace : %d\n\
    priv : %d\n",\
    fmt.fmt.pix.width,\
     fmt.fmt.pix.height,\
    fmt.fmt.pix.pixelformat,\
     fmt.fmt.pix.field, \
     fmt.fmt.pix.bytesperline, \
     fmt.fmt.pix.sizeimage, \
     fmt.fmt.pix.colorspace, \
     fmt.fmt.pix.priv);
    /* 得到摄像头所支持的所有格式 */
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf(" Support format : \n");
    while (ioctl(cam_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
        printf("\t%d.%s\n", fmtdesc.index+1, fmtdesc.description);
        fmtdesc.index++;
    }


    /* 向摄像头申请一个数据帧队列 */
    struct v4l2_requestbuffers req;
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    rel = ioctl(cam_fd, VIDIOC_REQBUFS, &req);
    if(rel < 0) {
        printf("request buffers error : %s\n", strerror(errno));
        goto ERROR;
    } else 
        printf("request buffers successed!\n");

    /* 申请存储图像缓冲区地址和长度的数组内存 */
    struct buffer * buffers = (struct buffer *)malloc(4*sizeof(struct buffer *));
    if (buffers == NULL ) {
        printf("malloc buffers err : %s\n", strerror(errno));
        goto ERROR;
    }
    /* 将缓冲区的内存映射到用户空间 */
    int n_buffers = 0;
    for (; n_buffers < req.count; n_buffers++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if( -1 == ioctl(cam_fd, VIDIOC_QUERYBUF, &buf)) {  //获取缓冲帧的地址
            printf("set buf error : %s\n", strerror(errno));
            goto ERROR;
        } else {
            printf("set buf success!\n");
        }
        buffers[n_buffers].length = buf.length;
        /* 映射内存空间 */
        buffers[n_buffers].start = mmap(NULL, buf.length, PROT_READ|PROT_WRITE, \
        MAP_SHARED, cam_fd, buf.m.offset);
        if (NULL == buffers[n_buffers].start) {
            printf("mmap error : %s\n", strerror(errno));
            goto MAP_ERROR;
        } else 
            printf("mmap success! address = %p\n",buffers[n_buffers].start);
            ioctl(cam_fd, VIDIOC_QBUF, &buf);
    }

    /* 开启视频流 */
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == ioctl(cam_fd, VIDIOC_STREAMON, &type))
            goto MAP_ERROR;
        else 
            printf("start stream!\n");


/*
 
*/
    /* 创建一个监听线程，用于停止程序 */
    pthread_t pd;
    pthread_create(&pd, NULL, listen, NULL);

    /* 开始捕获摄像头数据，并显示在屏幕上 */
    while(status) {
        /* 初始化select监听  */
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(cam_fd, &fds);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 125000;
        int ret = select(cam_fd+1, &fds, NULL, NULL, &tv);
        if (ret == -1) {
            printf(" error : listen failes\n");
        } else if (ret == 0) {
            printf(" time out !\n");
        } else {
            struct v4l2_buffer buf;
            memset(&buf, 0, sizeof(buf));
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            /* 得到一帧数据 */
            if (ioctl(cam_fd, VIDIOC_DQBUF, &buf) != -1) {
                /* 进行格式转换 */
                process_image(buffers[buf.index].start, bbf, buffers[buf.index].length);    
                /* 显示在屏幕上 */
                show_image(screen_bbf, bbf, width, height);
                /* 将帧放回队列 */
                if (-1 != (ioctl(cam_fd, VIDIOC_QBUF, &buf)))
                    printf(" put in success!\n");
                else 
                    printf(" put in failed!\n");
            } else 
                printf(" get frame failed!\n");
        }
    }

    /* 关闭视频流 */
    //enum v4l2_buf_type 
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(cam_fd, VIDIOC_STREAMOFF, &type);

    /* 解除内存映射，关闭文件描述符，程序结束 */
    int i;
    for (i=0; i<4; i++) {
        if (buffers[i].start != NULL)
            munmap(buffers[i].start, buffers[i].length);
    }
    close(cam_fd);
    fb_destory(fb_fd, screen_bbf, screenlen);
    free(bbf);
    return 0;


MAP_ERROR:  
    for (n_buffers=0; n_buffers<4; n_buffers++) {
        if (buffers[n_buffers].start != NULL)
            munmap(buffers[n_buffers].start, buffers[n_buffers].length);
    }
ERROR:
    fb_destory(fb_fd, screen_bbf, screenlen);
    close(cam_fd);
    free(bbf);
    return -1;
}
/* 
    不知道为什么以YUVY格式转换的RGB32在屏幕上的效果比YUYV格式展开的还要好
    下边程序中的是以YUVY格式展开的程序，YUYV的转换程序应该是
            v0 = yuv[count*4+0];
        y0 = yuv[count*4+1];
        u0 = yuv[count*4+2];
        y1 = yuv[count*4+3];
*/
void process_image(unsigned char * yuv, unsigned char * buf, int length)
{
    int count;
    int y0,u0,y1,v0;
    for (count=0; count<length/4; count++) {
        y0 = yuv[count*4+0];
        u0 = yuv[count*4+1];
        y1 = yuv[count*4+2];
        v0 = yuv[count*4+3];
        buf[count*8+0] = 1.164*(y0-16) + 2.018*(u0-128); //b
        buf[count*8+1] = 1.164*(y0-16) - 0.380*(u0-128) + 0.813*(v0-128); //g
        buf[count*8+2] = 1.164*(y0-16) + 1.159*(v0-128); //r
        buf[count*8+3] = 0; //透明度

        buf[count*8+4] = 1.164*(y1-16) + 2.018*(u0-128); //b
        buf[count*8+5] = 1.164*(y1-16) - 0.380*(u0-128) + 0.813*(v0-128); //g
        buf[count*8+6] = 1.164*(y1-16) + 1.159*(v0-128); //r
        buf[count*8+7] = 0; //透明度
    }
}

void show_image(char * screen_bbf, char * buf, int width, int height)
{
    int i,j;
    for (i=0; i<height; i++) {
            memcpy(screen_bbf+80*4+i*800*4 ,buf+i*width*4,width*4);
    }
}

char * fb_init(char * devname, int * fd, int * screenlen)
{
    *fd = open(devname, O_RDWR);
    if (*fd == -1) {
        printf("error : %s\n", strerror(errno));
        return NULL;
    }
    struct fb_var_screeninfo fbvs;
    int ret = ioctl(*fd, FBIOGET_VSCREENINFO, &fbvs);
    if (ret == -1) {
        printf("get screen info failed!\n");
        close(*fd);
        return NULL;
    } else {
        printf("screen info:\n\twidth : %d\thwight : %d\tbits_per_pixel : %d\n", \
        fbvs.xres, fbvs.yres,fbvs.bits_per_pixel);
    }

    *screenlen =  fbvs.xres*fbvs.yres*fbvs.bits_per_pixel/8;
    char *screen_bbf = (char *)mmap(NULL, *screenlen, PROT_WRITE | PROT_READ, MAP_SHARED, *fd,0);
    if (screen_bbf == NULL) {
        printf("screen mmap failed!\n");
        close(*fd);
        return NULL;
    } else {
        return screen_bbf;
    }
}

void fb_destory(int fd, char * screen_bbf, int screenlen) 
{
    munmap(screen_bbf, screenlen);
    close(fd);
}