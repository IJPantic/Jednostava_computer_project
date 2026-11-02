;Program odbrojavanja semafora
;2.11.2026. Ivan Jonjic (IJPantic na github)

.section text

_start:
    .setup
        int rst ;izađi iz interrupt moda

        mov sap, 1 ;postavi sektor semafor uređaja

        mov r0, 1
        mov $, r0 ;prikazi crveno
        wait 8

    .loop
        mov r0, r0, a>> ;shift udesno 1
        mov $, r0 ;prikazi zuto
        wait 8

        mov r0, r0, a>> ;shift udesno za 1
        mov $, r0 ;prikazi zeleno
        wait 8

        mov r0, a>> ;shift uljevo za 1
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov $, r0 ;prikazi zuto
        wait 2

        mov r0, a>> ;shift uljevo za 1
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov r0, a>>
        mov $, r0 ;prikazi crveno

        jmp .loop ;ponovi

