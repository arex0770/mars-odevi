
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int saglik, enerji, oksijen, su, parca, hasar;
int kilitMuhurlu = 0;
int gun = 1, tur = 0;

int zorluk = 2;         
int firtinaTur = 5;      
int firtinaHasar = 6;    
int olayBonus = 0;       
const char *zorlukAd = "Normal";

void tamponTemizle(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void sinirla(void)
{
    if (saglik < 0)   saglik = 0;
    if (saglik > 100) saglik = 100;
    if (enerji < 0)   enerji = 0;
    if (enerji > 100) enerji = 100;
    if (oksijen < 0)   oksijen = 0;
    if (oksijen > 100) oksijen = 100;
    if (hasar < 0)   hasar = 0;
    if (hasar > 100) hasar = 100;
    if (su < 0)    su = 0;
    if (parca < 0) parca = 0;
}

void zorlukSec(void)
{
    char secim;
    int gecersiz;

    do {
        gecersiz = 0;
        printf("Zorluk seviyesi secin (K: Kolay, N: Normal, Z: Zor): ");
        if (scanf(" %c", &secim) != 1)
            exit(0);
        tamponTemizle();

        switch (secim) {
        case 'k': case 'K':
            zorluk = 1; zorlukAd = "Kolay";
            saglik = 100; enerji = 100; oksijen = 100;
            su = 40; parca = 5; hasar = 0;
            firtinaTur = 3; firtinaHasar = 4; olayBonus = 10;
            break;
        case 'n': case 'N':
            zorluk = 2; zorlukAd = "Normal";
            saglik = 100; enerji = 90; oksijen = 100;
            su = 30; parca = 3; hasar = 10;
            firtinaTur = 5; firtinaHasar = 6; olayBonus = 0;
            break;
        case 'z': case 'Z':
            zorluk = 3; zorlukAd = "Zor";
            saglik = 90; enerji = 80; oksijen = 90;
            su = 20; parca = 2; hasar = 20;
            firtinaTur = 7; firtinaHasar = 9; olayBonus = -10;
            break;
        default:
            printf("Gecersiz seviye! Lutfen K, N veya Z girin.\n");
            gecersiz = 1;
        }
    } while (gecersiz);
}

int kesif(void)
{
    int zar, zar2, suEsik, parcaEsik;

    if (enerji < 15) {
        printf("Yetersiz enerji! Kesif icin en az 15 enerji gerekir. Komut iptal edildi.\n");
        return 0;
    }

    if (enerji > 40 && oksijen > 30)
        suEsik = 45;
    else
        suEsik = 20;
    suEsik += olayBonus;
    parcaEsik = suEsik + 25;

    enerji -= 15;
    oksijen -= 10;
    printf("Kesfe ciktiniz... (Enerji -15, Oksijen -10)\n");

    zar = rand() % 100;
    printf("[Zar] Yaralanma zari: %d | Kosul: zar < 10 || hasar > 70 (hasar = %d)\n", zar, hasar);
    if (zar < 10 || hasar > 70) {
        int kayip = 8 + 2 * zorluk;
        saglik -= kayip;
        printf("Yaralandiniz! Saglik -%d\n", kayip);
    } else {
        printf("Yaralanmadan dondunuz.\n");
    }

    zar2 = rand() % 100;
    printf("[Zar] Sonuc zari: %d | Su esigi: %d | Parca esigi: %d\n", zar2, suEsik, parcaEsik);
    if (zar2 < suEsik) {
        su += 6;
        printf("SONUC: Su kaynagi buldunuz! Su +6\n");
    } else if (zar2 < parcaEsik) {
        parca += 1;
        printf("SONUC: Enkaz icinde yedek parca buldunuz! Parca +1\n");
    } else {
        printf("SONUC: Eli bos dondunuz.\n");
    }
    return 1;
}

int oksijenUret(void)
{
    int miktar, artik, cift, maxCift, kullanilan, uretilen;

    if (enerji < 10) {
        printf("Yetersiz enerji! Oksijen uretimi icin 10 enerji gerekir. Komut iptal edildi.\n");
        return 0;
    }
    if (su < 2) {
        printf("Yetersiz su! En az 2 birim su gerekir. Komut iptal edildi.\n");
        return 0;
    }
    if (oksijen >= 100) {
        printf("Oksijen zaten %%100! Uretime gerek yok. Komut iptal edildi.\n");
        return 0;
    }

    printf("Kullanilacak su miktari (mevcut: %d): ", su);
    if (scanf("%d", &miktar) != 1) {
        tamponTemizle();
        printf("Gecersiz giris! Komut iptal edildi.\n");
        return 0;
    }
    tamponTemizle();

    if (miktar < 2 || miktar > su) {
        printf("Gecersiz miktar! 2 ile %d arasinda bir deger girin. Komut iptal edildi.\n", su);
        return 0;
    }

    artik = miktar % 2;               
    cift = (miktar - artik) / 2;     
    maxCift = (100 - oksijen) / 3;   
    if (cift > maxCift) {
        cift = maxCift;
        printf("Oksijen 100 sinirini asmamasi icin uretim sinirlandi.\n");
    }
    if (cift == 0) {
        printf("Depoda bu kadar oksijen icin yer yok. Komut iptal edildi.\n");
        return 0;
    }

    kullanilan = cift * 2;
    uretilen = cift * 3;
    su -= kullanilan;
    oksijen += uretilen;
    enerji -= 10;

    printf("%d su harcandi, %d oksijen uretildi. Enerji -10\n", kullanilan, uretilen);
    if (artik == 1)
        printf("Girilen miktar tek oldugu icin 1 birim su artik olarak depoda kaldi.\n");
    return 1;
}

int tamir(void)
{
    int parcaGerek, onarim, harcanan;

    if (parca <= 0) {
        printf("Yedek parca yok! Tamir yapilamaz. Komut iptal edildi.\n");
        return 0;
    }
    if (enerji < 10) {
        printf("Yetersiz enerji! Tamir icin 10 enerji gerekir. Komut iptal edildi.\n");
        return 0;
    }
    if (hasar <= 0) {
        printf("Moduller saglam, tamire gerek yok. Komut iptal edildi.\n");
        return 0;
    }

    if (hasar <= 30) {
        printf("Hasar seviyesi: HAFIF\n");
        parcaGerek = 1; onarim = 15;
    } else if (hasar <= 70) {
        printf("Hasar seviyesi: ORTA\n");
        parcaGerek = 2; onarim = 30;
    } else {
        printf("Hasar seviyesi: AGIR\n");
        parcaGerek = 3; onarim = 45;
    }

    if (parca >= parcaGerek) {
        harcanan = parcaGerek;
    } else {
        harcanan = parca;
        onarim = onarim * harcanan / parcaGerek;
    }

    parca -= harcanan;
    hasar -= onarim;
    enerji -= 10;
    printf("%d parca harcandi, hasar %d azaldi. Enerji -10\n", harcanan, onarim);
    return 1;
}

void durumRaporu(void)
{
    printf("\n===== DURUM RAPORU =====\n");
    printf("Gun: %d/15 | Tur: %d | Zorluk: %s\n", gun, tur, zorlukAd);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
    printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n", su, parca, hasar);
    printf("Hava kilidi: %s\n", kilitMuhurlu == 1 ? "MUHURLU" : "muhurlu degil");

    if (oksijen < 25) printf("!! KRITIK: Oksijen cok dusuk!\n");
    if (saglik < 30)  printf("!! KRITIK: Saglik cok dusuk!\n");
    if (hasar > 70)   printf("!! KRITIK: Modul hasari cok yuksek!\n");
    if (enerji < 20)  printf("!! KRITIK: Enerji cok dusuk!\n");
    if (su < 4)       printf("!! UYARI: Su stogu azaliyor!\n");
    if (parca == 0)   printf("!! UYARI: Yedek parca kalmadi!\n");

    if (saglik >= 30 && oksijen >= 25 && hasar <= 70 && enerji >= 20 && su >= 4 && parca >= 1)
        printf("TUM SISTEMLER NORMAL\n");
    printf("========================\n\n");
}

int dinlen(void)
{
    int enerjiArtis, saglikArtis;

    enerjiArtis = 30 - hasar / 10;    
    saglikArtis = 10 + enerji / 25;
    enerji += enerjiArtis;

    if (oksijen >= 20) {
        saglik += saglikArtis;
        printf("Dinlendiniz. Enerji +%d, Saglik +%d\n", enerjiArtis, saglikArtis);
    } else {
        if (oksijen < 10) {
            saglik -= 12;
            printf("Oksijen tehlikeli seviyede! Nefes alamadan uyudunuz. Enerji +%d, Saglik -12\n", enerjiArtis);
        } else {
            saglik -= 8;
            printf("Oksijen az oldugu icin dinlenme sagliginizi bozdu. Enerji +%d, Saglik -8\n", enerjiArtis);
        }
    }
    oksijen -= 3;   
    printf("Oksijen tuketimi: -3\n");
    return 1;
}

int firtina(void)
{
    int i, zar, carpan, ekstra, canEsik, oksEsik, hasEsik;
    int oncS, oncO, oncH, zararli = 0, topS = 0, topO = 0, topH = 0;

    if (enerji < 10) {
        printf("Yetersiz enerji! Firtinaya hazirlanmak icin 10 enerji gerekir. Komut iptal edildi.\n");
        return 0;
    }
    enerji -= 10;
    printf("KUM FIRTINASI basliyor! Sure: %d tur (Enerji -10)\n", firtinaTur);

    for (i = 1; i <= firtinaTur; i++) {
        zar = rand() % 100;
        carpan = (i % 2 == 0) ? 2 : 1;   

        ekstra = 0;
        if (enerji < 30) ekstra += 10;
        if (hasar > 70)  ekstra += 10;
        canEsik = 20 + ekstra;
        oksEsik = canEsik + 20;
        hasEsik = oksEsik + 20;

        printf("-- Firtina turu %d%s --\n", i, carpan == 2 ? " (RUZGAR SIDDETLENDI, hasar x2)" : "");
        printf("[Zar] %d | Can < %d | Oksijen sizintisi < %d | Modul hasari < %d | Diger: korunma\n",
               zar, canEsik, oksEsik, hasEsik);

        oncS = saglik; oncO = oksijen; oncH = hasar;

        if (zar < canEsik) {
            saglik -= firtinaHasar * carpan;
            printf("Savrulan kumlar sizi yaraladi! Saglik -%d\n", firtinaHasar * carpan);
        } else if (zar < oksEsik) {
            if (kilitMuhurlu == 1) {
                printf("Hava kilidi muhurlu, oksijen sizintisi bu tur atlandi.\n");
                continue;
            }
            oksijen -= firtinaHasar * carpan;
            printf("Oksijen sizdi! Oksijen -%d\n", firtinaHasar * carpan);
        } else if (zar < hasEsik) {
            hasar += firtinaHasar * carpan;
            printf("Modul hasar aldi! Hasar +%d\n", firtinaHasar * carpan);
        } else {
            printf("Firtinadan zararsiz korundunuz.\n");
        }

        sinirla();

        if (saglik < oncS || oksijen < oncO || hasar > oncH)
            zararli++;
        topS += oncS - saglik;
        topO += oncO - oksijen;
        topH += hasar - oncH;

        if (saglik <= 0 || oksijen <= 0) {
            printf("Kritik seviye! Firtina aninda hayati tehlike olustu.\n");
            break;
        }
    }

    printf("--- Firtina ozeti: %d turda hasar alindi | Kaybedilen saglik: %d | Kaybedilen oksijen: %d | Modul hasari: +%d ---\n",
           zararli, topS, topO, topH);
    return 1;
}

int havaKilidi(void)
{
    char sifre, giris = '?';
    int deneme = 0;

    if (kilitMuhurlu == 1) {
        printf("Hava kilidi zaten muhurlu. Komut iptal edildi.\n");
        return 0;
    }
    if (enerji < 15) {
        printf("Yetersiz enerji! 3 deneme icin en az 15 enerji gerekir. Komut iptal edildi.\n");
        return 0;
    }

    sifre = 'A' + (gun % 26);
    printf("IPUCU: Sifre, alfabede A harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);

    do {
        printf("Deneme %d/3 - Sifre (A-Z): ", deneme + 1);
        if (scanf(" %c", &giris) != 1)
            giris = '?';
        else
            tamponTemizle();

        if (giris >= 'a' && giris <= 'z')
            giris = giris - 'a' + 'A';  

        deneme++;

        if (giris != sifre) {
            enerji -= 5;
            if (enerji < 0) enerji = 0;
            printf("Yanlis sifre! Enerji -5. ");
            if (giris < 'A' || giris > 'Z')
                printf("(Gecersiz karakter)\n");
            else if (giris < sifre)
                printf("Ipucu: ileri git\n");
            else
                printf("Ipucu: geri git\n");
        }
    } while (giris != sifre && deneme < 3);

    if (giris == sifre) {
        kilitMuhurlu = 1;
        printf("Dogru sifre! Hava kilidi MUHURLENDI.\n");
    } else {
        hasar += 20;
        printf("3 deneme basarisiz! Kilit zorlandi, modul hasari +20\n");
    }
    return 1;
}

void sonucRaporu(int sonuc)
{
    int hayattaGun, puan;

    hayattaGun = (gun > 15) ? 15 : gun;
    puan = (hayattaGun * 100) + saglik + oksijen - hasar;

    printf("\n=========== SONUC RAPORU ===========\n");
    if (sonuc == 1)
        printf("KAZANDINIZ! Kurtarma ekibi geldi.\n");
    else if (sonuc == -1)
        printf("KAYBETTINIZ! Hayatta kalamadiniz.\n");
    else
        printf("Oyundan cikildi.\n");
    printf("Zorluk: %s\n", zorlukAd);
    printf("Hayatta kalinan gun: %d\n", hayattaGun);
    printf("Toplam tur: %d\n", tur);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
    printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n", su, parca, hasar);
    printf("PUAN = (gun x 100) + saglik + oksijen - hasar = %d\n", puan);
    printf("====================================\n");
}

int main(void)
{
    char komut;
    int devam = 1;
    int sonuc = 0;    
    int gecerli;

    srand(time(NULL));

    printf("=== MARS KOLONISI YASAM DESTEK SIMULATORU ===\n");
    printf("Kurtarma ekibi gelene kadar 15 gun hayatta kalin!\n\n");
    zorlukSec();
    printf("\nZorluk: %s secildi. Iyi sanslar!\n", zorlukAd);

    do {
        printf("\n[Gun %d | Tur %d] Komut (G:Kesif O:Oksijen T:Tamir D:Durum U:Uyu K:Firtina H:Kilit Q:Cikis): ",
               gun, tur);
        if (scanf(" %c", &komut) != 1)
            komut = 'Q';  
        else
            tamponTemizle();

        gecerli = 0;

        switch (komut) {
        case 'g': case 'G':
            gecerli = kesif();
            break;
        case 'o': case 'O':
            gecerli = oksijenUret();
            break;
        case 't': case 'T':
            gecerli = tamir();
            break;
        case 'd': case 'D':
            durumRaporu();         
            break;
        case 'u': case 'U':
            gecerli = dinlen();
            break;
        case 'k': case 'K':
            gecerli = firtina();
            break;
        case 'h': case 'H':
            gecerli = havaKilidi();
            break;
        case 'q': case 'Q':
            devam = 0;
            sonuc = 0;
            break;
        default:
            printf("Taninmayan komut '%c'! Tur harcanmadi.\n", komut);
        }

        if (gecerli == 1) {
            tur++;
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n*** YENI GUN BASLADI: Gun %d *** (Oksijen -8)\n", gun);
                if (hasar > 70) {
                    saglik -= 5;
                    printf("Modul hasari yuksek! Saglik -5\n");
                }
            }
        }

        sinirla();   

       
        if (devam == 1) {
            if (saglik <= 0 || oksijen <= 0) {
                devam = 0;
                sonuc = -1;
            } else if (gun > 15) {
                devam = 0;
                sonuc = 1;
            }
        }
    } while (devam);

    sonucRaporu(sonuc);
    return 0;
}
