//
//  xor.c
//  XOR Problemi - Yapay Sinir Ağları
//
//  Created by Emre Jılta on 27/04/17.
//  
//

#include <stdio.h>
#include <math.h>

int noron1[] = {0,0,1,1}, noron2[] = {0,1,0,1};
float gizliKatman1, gizliKatman2;
int biasGizli=1, biasCikis=1;
float agirlik[] = {0.129952, 0.570345, -0.923123, -0.328932, 0.341232, -0.115223, 0.164732, 0.752621, -0.993423};  //A11, A12, A21, A22, A31, A32, A41, A51, A61

/*
 1)
 noron1, A11 ve A12
 noron2, A21 ve A22
 biasGizli, A31 ve A32

 1.1) net1 = noron1*A11[0] + noron2*A21[2] + biasGizli*A31[4]
 1.2) net2 = noron1*A12[1] + noron2*A22[3] + biasGizli*A32[5]

 2)
 net1, A41
 net2, A51
 biasCikis, A61

 2.1) net3 = net1*A41[6] + net2*A51[7] + biasCikis*A61[8]
 */

float netAgirlik(int n, float a[], float w[]){
    int i;
    float net;
    for(i=0; i<n; i++){
        net += (a[i]*w[i]);
    }
    return net;
}

//Sigmoid çıkış değeri
float sigmoid(float ex){
    return (1 / (1 + pow(M_E,-ex)));
}

//Ağdaki hata
float agHata(float x){
    return (0-x);
}

//Hücredeki değişim miktarı
float degisimMiktari(float d){
    return (d*(1-d)*agHata(d));
}

//Hata
float hata(float lambda, float momentum, float degisim, float dugum, float deger){
    return (lambda*degisim*dugum)+(momentum*deger);
}

//Ağırlık ve hata
float agirlikDegisim(float a, float b){
    return (a + b);
}

//Hata oranı
float hataOrani(float sigmoid, float degisimMiktari, float agirlik){
    return sigmoid*(1-sigmoid)*degisimMiktari*agirlik;
}

//Ara katman değişim miktarı
float araKatmanDegisimMiktari(float lambda, float hataOrani, float dugum, float momentum, float deger){
    return (lambda*hataOrani*dugum)+(momentum*deger);
}

//Güncel ağırlıklar
float guncelAgirlik(float agirlik, float degisimMiktari){
    return agirlik+degisimMiktari;
}

int main(){
    float net1 = 0, net1_t = 0, net1_s = 0, net1_h = 0, net2 = 0, net2_t = 0, net2_s = 0, net2_h = 0, net3 = 0, net3_t = 0, net3_s = 0, degisim_t = 0; //net, ağırlık hesabı; net_t, toplam ağırlık; net_s, sigmoid/çıkış, degisim_t, çıkış değerinin değişim miktarının toplamı; net_h, hata oranı
    float h1 = 0, h2 = 0, h3 = 0, hata_t = 0, h1_d = 0, h2_d = 0, h3_d = 0;
    float ogrenmeKatsayisi = 0.5, momentum = 0.8;  //lambda ve alfa
    float degisimmiktari = 0, hOrani = 0;
    float guncelDeger1[3] = {}, guncelDeger2[3] = {};
    int i,j,k;
    float giris[3] = {0,0,1};

    printf("net1 \t\t sigmoid \t net2 \t\t sigmoid \t net3 \t\t sigmoid \t degisim\n");

    for (i=0; i<4; i++) {
        net1 = (noron1[i]*agirlik[0] + noron2[i]*agirlik[2] + biasGizli*agirlik[4]);
        net1_t += net1;
        net1_s += sigmoid(net1);
        float ns1 = sigmoid(net1);

        net2 = (noron1[i]*agirlik[1] + noron2[i]*agirlik[3] + biasGizli*agirlik[5]);
        net2_t += net2;
        net2_s += sigmoid(net2);
        float ns2 = sigmoid(net2);

        net3 = (sigmoid(net1)*agirlik[6] + sigmoid(net2)*agirlik[7] + biasCikis*agirlik[8]);
        net3_t += net3;
        net3_s += sigmoid(net3);

        degisimmiktari = degisimMiktari(sigmoid(net3));

        h1 = hata(ogrenmeKatsayisi, momentum, degisimmiktari, ns1, 0);
        h1_d = agirlikDegisim(agirlik[6],h1);
        net1_h = hataOrani(ns1,degisimmiktari,agirlik[6]);

        h2 = hata(ogrenmeKatsayisi, momentum, degisimmiktari, ns2, 0);
        h2_d = agirlikDegisim(agirlik[7],h2);
        net2_h = hataOrani(ns2,degisimmiktari,agirlik[7]);

        h3 = hata(ogrenmeKatsayisi, momentum, degisimmiktari, biasCikis, 0);
        h3_d = agirlikDegisim(agirlik[8],h1);

        for(j=0; j<4; j++){
            for(k=0; k<4; k++){
                guncelDeger1[j] += araKatmanDegisimMiktari(ogrenmeKatsayisi, net1_h, giris[k], momentum, 0);
                guncelDeger2[j] += araKatmanDegisimMiktari(ogrenmeKatsayisi, net2_h, giris[k], momentum, 0);
            }
        }



        printf("%lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf\n",net1,sigmoid(net1),net2,sigmoid(net2),net3,sigmoid(net3),degisimmiktari);

        degisim_t += degisimmiktari;
        hata_t += (h1+h2+h3);

    }

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("%lf \t %lf \t %lf \t %lf \t %lf \t %lf \t %lf \n",net1_t,net1_s,net2_t,net2_s,net3_t,net3_s,degisim_t);

    printf("\n\n");

    printf("hata1 \t\t degisim \t hata2 \t\t degisim \t hata3 \t\t degisim\n");
    printf("%lf \t %lf \t %lf \t %lf \t %lf \t %lf \n",h1,h1_d,h2,h2_d,h3,h3_d);

    return 0;
}
