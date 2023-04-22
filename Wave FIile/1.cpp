#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

float getLength(int eumpyo) {
    float T;
    if (eumpyo == 4) {
        T = 1.0;
    }
    else if (eumpyo == 8) {
        T = 0.5;
    }
    else if (eumpyo == 16) {
        T = 0.25;
    }
    return T;
}

float getFrequency(char hh) {
    float ff;
    if (hh == 'C') {
        ff = 261.63;
    }
    else if (hh == 'E') {
        ff = 329.63;
    }
    else if (hh == 'G') {
        ff = 392.00;
    }
    return ff;
}

float getAmplitude(int ss) {
    int aa;
    if (ss == 10) {
        aa = 10000;
    }
    else if (ss == 5) {
        aa = 5000;
    }
    else if (ss == 8) {
        aa = 8000;
    }
    return aa;
}

int main() {
    char header[44];
    int *samplerate, *byteRate; //samplerate = fs, byteRate = b
    short *NumChannels, *bitsperS; //NumChannels = 

    ifstream musicfile;
    musicfile.open("musicfile.wav", ios::binary | ios::in);
    if (!musicfile) return 777;
    musicfile.read(header, sizeof(header));
    musicfile.close();

    NumChannels = (short*)(header + 22);
    cout << *NumChannels << endl;
    *NumChannels = 1;
    cout << *NumChannels << endl;

    samplerate = (int*)(header + 24);
    cout << *samplerate << endl;

    byteRate = (int*)(header + 28);
    cout << *byteRate << endl;
    *byteRate = NumChannels[0] * samplerate[0] * sizeof(short);
    cout << *byteRate << endl;

    bitsperS = (short*)(header + 34);
    cout << *bitsperS << endl;

    int N = samplerate[0] * 10;
    short* data;
    data = new short[N];

    musicfile.open("musicfile.wav", ios::binary | ios::in);
    musicfile.read((char*)data, N * sizeof(short));
    musicfile.close();

    ofstream xx;
    xx.open("my.wav", ios::binary | ios::out);
    if (!xx) return 888;
    xx.write(header, sizeof(header));
    xx.write((char*)data, N * sizeof(short));
    xx.close();

    short* data1;
    data1 = new short[samplerate[0]];

    const float pi = 3.141492;
    float dt = 1. / samplerate[0];
    float a = 10000.0;
    float f = 440.0;

    for (int i = 0; i < samplerate[0]; i++) {
        data1[i] = (short)(a * sin(2.0 * pi * f * i * dt));
    }

    xx.open("my.wav", ios::binary | ios::out);
    if (!xx) return 666;
    xx.write((char*)header, 44 * sizeof(char));
    xx.write((char*)data1, samplerate[0] * sizeof(short));
    xx.close();

    int K = samplerate[0] * 5;
    short* data2;
    data2 = new short[K];
    int n;
    float *FF, *TT;
    int* AA, *EU, *SS;
    char *HH;

    ofstream msfile("music.wav", ios::binary | ios::out);
    if (!msfile) return 777;
    msfile.write((char*)header, 44 * sizeof(char));

    ifstream zz("piece.txt");
    if (!zz) return 555;
    zz >> n;
    FF = new float[n];
    TT = new float[n];
    AA = new int[n];
    EU = new int[n]; 
    SS = new int[n];
    HH = new char[n];
    for (int i = 0; i < n; i++) {
        zz >> EU[i] >> HH[i] >> SS[i];
        FF[i] = getFrequency(HH[i]);
        AA[i] = getAmplitude(SS[i]);
        TT[i] = getLength(EU[i]);
        for (int j = 0; j < samplerate[0] * TT[i]; j++) {
            data2[j] = (short)(AA[i] * sin(2.0 * pi * FF[i] * j * dt));
        }
        msfile.write((char*)data2, samplerate[0] * TT[i] * sizeof(short));
    }
    zz.close();
    msfile.close();
}