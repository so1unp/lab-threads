## Threads a nivel de usuario en xv6

## Instalación
Copien los archivos `uthread.c` y `uthread_switch.S` en su directorio de `xv6`. Copiar también el archivo `Makefile`, o agregar lo siguiente al actual:
```
_uthread: uthread.o uthread_switch.o
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _uthread uthread.o uthread_switch.o $(ULIB)
	$(OBJDUMP) -S _uthread > uthread.asm
```

## Ejecución

