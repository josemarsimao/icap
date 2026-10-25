# icap

Captura de imagem de uma câmera V4L2 no Linux, com exibição em arte ASCII no terminal. Cada frame é convertido para tons de cinza (`v4l2gray`) e desenhado como texto. É o exemplo de uso da fachada de câmera única do [v4l2misc](https://github.com/josemarsimao/v4l2misc).

## Dependências

- Repositórios irmãos [v4l2misc](https://github.com/josemarsimao/v4l2misc) e [kbhit](https://github.com/josemarsimao/kbhit), na mesma pasta-mãe:

  ```
  projetos/
  ├── icap/
  ├── v4l2misc/
  └── kbhit/
  ```

- `libjpeg` (para frames MJPEG).

Não usa OpenCV.

## Compilação

Abra `icap.cbp` no Code::Blocks e compile (`bin/Debug/icap` ou `bin/Release/icap`).

## Uso

```
icap [opções]

-d | --device nome   nome do dispositivo de vídeo
-h | --help          mostra a ajuda
-m | --mmap          buffers mapeados em memória [padrão]
-r | --read          usa chamadas read()
-u | --userp         buffers alocados pela aplicação
-o | --output        envia o fluxo bruto para stdout
-f | --format        força 640x480 YUYV
-c | --count         número de frames a capturar [70]
-s | --clist         mostra a lista de controles da câmera
```

Ao iniciar, o programa lista os dispositivos e pergunta qual usar e qual formato configurar.

## Autor

Josemar Simão (josemars@ifes.edu.br)
