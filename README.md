# GhostOS

Versione corrente: v1.2-alpha

Ghost OS e' un piccolo sistema operativo educativo in Assembly NASM
16-bit real mode. Il kernel e' diviso in moduli `.inc`, la shell include
una base storica 16-bit con FAT12/`.GHT` e ora avvia una shell 32-bit
modulare con paging, heap minimale e task abstraction cooperativa.
La shell `ghost32>` include lettura FAT12 read-only per `ls`, `cat` e
`browse`, senza rete o stack HTTP.
La fase corrente stabilizza FAT12 in protected mode e aggiunge diagnostica
per heap, pagine fisiche e mappa E820.
