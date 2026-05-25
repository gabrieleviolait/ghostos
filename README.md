# GhostOS

Versione corrente: v1.5-alpha

Ghost OS e' un piccolo sistema operativo educativo in Assembly NASM
16-bit real mode. Il kernel e' diviso in moduli `.inc`, la shell include
una base storica 16-bit con FAT12/`.GHT` e ora avvia una shell 32-bit
modulare con paging, heap minimale e task abstraction cooperativa.
La shell `ghost32>` include lettura FAT12 read-only per `ls`, `cat` e
`browse`, senza TCP/IP, DNS o stack HTTP.
La fase corrente aggiunge fondamenta PCI con `lspci` e rilevamento NIC
tramite `netdev`, senza inizializzare trasmissione, ricezione o interrupt NIC.
La fase precedente stabilizza l'ABI runtime `.GEX` e aggiunge diagnostica
protetta per registri, stack, uptime e heap.
La shell `ghost32>` include il loader `.GEX` con `run HELLO.GEX [args]`,
syscall stabili `print`, `read`, `exit`, `get_version`, `get_ticks` e
ritorno sicuro alla CLI con codice di uscita.
La shell protected ora ha editing minimale della riga, history e tab
completion dei comandi noti.
La fase v1.5-alpha resta CLI-only: solo discovery PCI/NIC, nessun pacchetto
RX/TX e nessun livello TCP/IP o HTTP.

## QEMU NIC examples

RTL8139:

```sh
qemu-system-i386 -drive file=build/ghostos.img,format=raw,if=floppy -vga std -netdev user,id=n0 -device rtl8139,netdev=n0
```

E1000:

```sh
qemu-system-i386 -drive file=build/ghostos.img,format=raw,if=floppy -vga std -netdev user,id=n0 -device e1000,netdev=n0
```
