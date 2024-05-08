# Proiect2-SD

Vă încântăm cu un alt proiect deosebit marca Roxana Asavei, Teodor Lepădatu și Alexandru Țîncu. Au fost implementate și testate următoarele structuri de date:
- AVL
- BTree (cu t = 4)
- Treap (cu prioritatea generată aleator)

Pentru fiecare, am implementat operațiile de inserare, ștergere și căutare (exactă, cel mai mare număr mai mic decât query și cel mai mic număr mai mare decât query) - mai precis, operațiile de pe [infoarena](https://www.infoarena.ro/problema/abce). Algoritmii originali au luat toți 100 de puncte. Algoritmii originali se găsesc în directorul extra/.

### Testare

Pentru testare, a fost folosită o abordare orientată obiect, cu o clasă pentru fiecare structură de date.

Testele au fost rulate folosind scriptul test.sh. Se va utiliza astfel:
```
./test.sh nr_rulări
```

Pentru fiecare rulare, va fi rulat main.cpp cu câte 10^3, 10^5 și 10^6 numere, iar funcțiile de căutare vor fi testate pentru corectitudine utilizând funcția bash cmp.

În cadrul unei executări ale fișierului main, sunt generate două șiruri a câte n numere astfel: se genereaza toate numerele din invervalul [-n\*2, n\*2], se dă shuffle, și se elimină ultimele n\*3 numere, astfel garantând unicitatea și având niște șiruri de numere dezordonate și "spațiate".

Au fost făcute câte 12 rulări pentru 10^3, 10^5 și 10^6 numere, pentru fiecare structură de date. Având în vedere viteza operațiilor de tip insert și erase, au fost rulate teste separate cu cod comentat în main.cpp și în test.sh pentru a testa 10^8 inserări și ștergeri.

### Linkuri

link prezentare: https://tinyurl.com/prezentarecopacei

link date teste: https://tinyurl.com/datecopacei

### Licență
Acest proiect este licențiat sub licența [MIT](https://opensource.org/licenses/MIT). Pentru mai multe detalii, consultați fișierul LICENSE.md.