void rng(int* arr, int n) {
    int seed = 13515000; // Ganti dengan NIM anda sebagai seed.
    srand(seed);
    for(long i = 0; i < n; i++) {
        arr[i] = (int)rand();
    }
}

int main (){
	

	return 0;
}