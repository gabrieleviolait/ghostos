# GhostOS

Versione corrente: v0.9

Ghost OS e' un piccolo sistema operativo educativo in Assembly NASM
16-bit real mode. Il kernel e' diviso in moduli `.inc`, la shell include
una base storica 16-bit con FAT12/`.GHT` e ora avvia una shell minimale
32-bit in protected mode senza BIOS per video o tastiera.
