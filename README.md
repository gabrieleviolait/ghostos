# GhostOS

Versione corrente: v1.4.5-alpha

Ghost OS e' un piccolo sistema operativo educativo in Assembly NASM
16-bit real mode. Il kernel e' diviso in moduli `.inc`, la shell include
una base storica 16-bit con FAT12/`.GHT` e ora avvia una shell 32-bit
modulare con paging, heap minimale e task abstraction cooperativa.
La shell `ghost32>` include lettura FAT12 read-only per `ls`, `cat` e
`browse`, senza rete o stack HTTP.
La fase corrente stabilizza l'ABI runtime `.GEX` e aggiunge diagnostica
protetta per registri, stack, uptime e heap.
La shell `ghost32>` include il loader `.GEX` con `run HELLO.GEX [args]`,
syscall stabili `print`, `read`, `exit`, `get_version`, `get_ticks` e
ritorno sicuro alla CLI con codice di uscita.
La shell protected ora ha editing minimale della riga, history e tab
completion dei comandi noti.
La fase v1.4.5-alpha resta CLI-only e prepara il runtime prima di qualunque
stack di rete.
