# Praktikum 1 - IF3230 - Bitonic Sort
Tugas OpenMP - Bitonic Sort. Praktikum1_K01_13515007_13515037
Dzar Bela Hanifa - 13515007
Muhamad Irfan Maulana - 13515037

### Petunjuk Penggunaan Program

1. Buka terminal
2. Jalankan command di bawah ini
    ```sh
    make
    ```
3. Jalankan program dan masukkan jumlah data yang ingin diurutkan
    ```sh
    ./bitonic_sort <jumlah_data>
    ```
### Pembagian Tugas
| NIM | Tugas |
| ------ | ------ |
| 13515007 | Paralelisasi, implementasi sort, padding input, random number generator|
| 13515037 | Paralelisasi, testing, debugging, print ke file eksternal, melakukan perhitungan waktu |

## Laporan Pengerjaan
### Deskripsi Solusi Paralel
*Bitonic sort* merupakan algoritma *sorting* yang digunakan untuk komputasi paralel. Algoritma ini memiliki kompleksitas O(n log^2 (n) ) untuk proses komparasi dan O(log^2(n)) *delay*. Untuk mengimplementasi algoritma ini, kami menambahkan line berikut pada program kami
```sh
#pragma omp parallel for num_threads(jumlah_thread)
```
Contoh program yang diberikan asisten hanya mampu menerima data bilangan sebesar 2^N, oleh karena itu kami melakukan beberapa penyesuaian agar program dapat menerima input yang bukan merupakan bilangan 2^N. Penyesuaian tersebut adalah menambahkan data *dummy* pada *array* yang akan di-*sort* dengan **nilai lebih besar** atau **sama** dengan data terbesar yang terdapat pada *array* sebelumnya. Total jumlah data setelah penambahan data *dummy* haruslah merupakan bilangan 2^N. Data *dummy* tidak akan berpengaruh pada *sorting* karena pasti akan terletak di bagian belakang *array*.

### Analisis Solusi
Solusi yang telah diberikan dengan menggunakan *bitonic sort* yang telah di paralelisasi merupakan solusi yang sudah efektif. Hal ini dikarenakan perbandingan yang dilakukan secara paralel dan bisa menggunakan lebih dari 1 thread. Jadi, menurut kami solusi ini sudah cukup baik untuk menyelesaikan masalah tersebut.

### Jumlah *thread* yang Digunakan
Jumlah *thread* yang digunakan adalah dua. Kami menyesuaikan jumlah *thread* dengan jumlah *core* yang terdapat pada prosesor yang kami gunakan. Jika *thread* terlalu banyak, maka akan terjadi *overhead*.

### Pengukuran Kinerja untuk Tiap Kasus Uji
Berikut adalah hasil uji pengukuran kinerja dengan menggunakan *bitonic sort* :
| N | Waktu Paralel | Waktu Serial |
| ------ | ------ | ------ |
| 5000 (1) | 0.002249 | 0.002640 |
| 5000 (2) | 0.002645 | 0.002852 |  
| 5000 (3) | 0.002903 | 0.002496 |   
| 50000 (1) | 0.021233  | 0.021737 |   
| 50000 (2) | 0.020336 | 0.027175 |    
| 50000 (3) | 0.020146 | 0.022016 |  
| 100000 (1) | 0.035204 | 0.048447 |   
| 100000 (2) | 0.035165 | 0.049440 |    
| 100000 (3) | 0.035674 | 0.049457 |   
| 200000 (1) | 0.075483 | 0.106529 |   
| 200000 (2) | 0.074658 | 0.106871 |   
| 200000 (3) | 0.074433 | 0.106354 |   
| 400000 (3) | 0.164329 | 0.243308 |   
| 400000 (3) | 0.164514 | 0.236701 | 
| 400000 (3) | 0.163929 | 0.235708 |   

### Analisis Perbandingan Kinerja
Berdasarkan data percobaan yang telah dilakukan, kinerja *bitonic sort* paralel lebih baik dibanding *bitonic sort* serial. Percobaan dengan jumlah elemen sebanyak 5000, 50000, 100000, 200000, dan 400000 berturut-turut mempunyai rata-rata peningkatan kinerja sebesar 1.037x, 1.151x, 1.389x, 1.424x, dan 1.452x. Berdasarkan data tersebut, perbedaan kinerja akan terasa signifikan apabila data yang diurutkan berjumlah banyak.


