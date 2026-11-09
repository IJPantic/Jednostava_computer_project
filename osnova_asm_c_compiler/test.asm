;Program odbrojavanja semafora za pjesake i promet koristeci interrupt
;7.11.2026. Ivan Jonjic (IJPantic na github-u)

.section text

_start:
    .setup
        int rst ;izađi iz interrupt moda

        mov sap, 1 ;postavi sektor semafor uređaja

        mov pci, .loop_pedestrian

    .loop_traffic
        mov a, 9 ;postavi semafor za pjesake i za promet crveno 
        mov ap$, a

        mov a, 10 ;postavi semafor za pjesake crveno, a za promet zuto 
        mov ap$, a

        mov a, 12 ;postavi semafor za pjesake crveno, a za promet zeleno 
        mov ap$, a

        mov a, 10 ;postavi semafor za pjesake crveno, a za promet zuto 
        mov ap$, a

        jmp .loop_traffic ;ponovi

    .loop_pedestrian
        mov a, 17 ;postavi semafor za pjesake zeleno, a za promet crveno 
        mov ap$, a

        mov pc, setup ;vrati se nazad

