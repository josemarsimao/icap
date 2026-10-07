/*
 *  V4L2 video capture example
 *
 *  Nome: Josemar Simão
 *  email: josemars@ifes.edu.br
 *
 *  This program can be used and distributed without restrictions.
 *
 *  This program is provided with the V4L2 API
 *  see http://linuxtv.org/docs.php for more information
 *
 */

#include <getopt.h>             /// getopt_long()




#include "v4l2misc.h"


static int out_buf = 0;
static int clist = 0;
#define LEGACY_PROCESS 0

#if(LEGACY_PROCESS == 1)
void process_image(const void *p, int isize) {

    if (out_buf)
        fwrite(p, isize, 1, stdout);

    fflush(stderr);

    asciiart(p,isize,h,w);

    fflush(stdout);

}
#else
void process_image(const void *p, int isize) {

    // Converte qualquer formato de hardware para Tons de Cinza 8-bit padrão
    unsigned char* standardized_gray_data = convert_to_grayscale((unsigned char*)p, isize);

    if (standardized_gray_data) {

        // O asciiart agora é universal: só aceita buffers contíguos de brilho puro
        asciiart(standardized_gray_data, h * w, h, w);

        // Libera a memória alocada dinamicamente pelo conversor correspondente
        free(standardized_gray_data);
    }

    fflush(stdout);
}
#endif

/* Sequencia:
        1 - Utilizar ( struct v4l2_requestbuffers / VIDIOC_REQBUFS ) para requisitar uma quantidade de buffers
        2 - Verificar qual quantidade foi atendida ( req.count )
        3 - Utilizar ( struct v4l2_buffer / VIDIOC_QUERYBUF ) para pegar informações do buffer alocado pelo driver:
                 3.1 - comprimento da imagem (buffer size)
                 3.2 - mapeamento de endereços do espace do driver para o espaço do usuário
        4 -



*/



static int              frame_count = 70;
static void usage(FILE *fp, int argc, char **argv, char* dv_nm)
{
        fprintf(fp,
                 "Usage: %s [options]\n\n"
                 "Version 1.3\n"
                 "Options:\n"
                 "-d | --device name   Video device name [%s]\n"
                 "-h | --help          Print this message\n"
                 "-m | --mmap          Use memory mapped buffers [default]\n"
                 "-r | --read          Use read() calls\n"
                 "-u | --userp         Use application allocated buffers\n"
                 "-o | --output        Outputs stream to stdout\n"
                 "-f | --format        Force format to 640x480 YUYV\n"
                 "-c | --count         Number of frames to grab [%i]\n"
                 "-s | --clist         Show user controls list\n"
                 "",
                 argv[0], dv_nm, frame_count);
}


static const char short_options[] = "d:hmruofc:s";


static const struct option long_options[] = {

        { "device", required_argument, NULL, 'd' },
        { "help",   no_argument,       NULL, 'h' },
        { "mmap",   no_argument,       NULL, 'm' },
        { "read",   no_argument,       NULL, 'r' },
        { "userp",  no_argument,       NULL, 'u' },
        { "output", no_argument,       NULL, 'o' },
        { "format", no_argument,       NULL, 'f' },
        { "count",  required_argument, NULL, 'c' },
        { "clist",  no_argument,       NULL, 's' },
        { 0, 0, 0, 0 }

};


void set_options(int argc, const char** argv, const char* shortopts, const struct option* longopts){

    for (;;) {

        int idx;
        int c;

        c = getopt_long(argc, argv, shortopts, longopts, &idx);

        if (-1 == c)
                break;

        switch (c) {
        case 0:
            break;

        case 'd':
            strcpy(get_dev_name(),optarg);
            break;

        case 'h':
            usage(stdout, argc, argv, get_dev_name());
            exit(EXIT_SUCCESS);

        case 'm':
            io = IO_METHOD_MMAP;
            break;

        case 'r':
            io = IO_METHOD_READ;
            break;

        case 'u':
            io = IO_METHOD_USERPTR;
            break;

        case 'o':
            out_buf++;
            break;

        case 'f':
            force_format++;
            break;

        case 'c':
            errno = 0;
            frame_count = strtol(optarg, NULL, 0);
            if (errno)
                errno_exit(optarg);
            break;
        case 's':
            errno = 0;
            clist = 1;
            if (errno)
                errno_exit(optarg);
            break;

        default:
            usage(stderr, argc, argv, get_dev_name());
            exit(EXIT_FAILURE);

        }

    }

}

int main(int argc, char **argv){

    int file_id;

    set_options(argc,argv,short_options,long_options);

    init_terminal();

    choose_v4l2_device();

    file_id = open_v4l2_device(0);

        setting_video_format(file_id);
        init_device(file_id); /// Aqui configura formato e resolução escolhidos init_userp, init_mmap, init_read, init_dmabuf

                /// allocate_image(h, w); /// Utilizado para exibição em janela gráfica
                /// fill_lookuptables();
                /// namedWindow("Frame",1);
            start_capturing(file_id);
                mainloop(file_id);
            stop_capturing(file_id);
        uninit_device(file_id); /// libera buffers alocados

    close_v4l2_device(file_id);

    ///deallocate_image();


    fprintf(stderr, "\n");

    return 0;
}
