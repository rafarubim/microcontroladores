#pragma once

template <typename T, unsigned N>
inline unsigned arraySize(const T(&)[N]) {
  return N;
}

template <typename T, unsigned N_DEST, unsigned N_SOURCE>
inline void arrayCopy(T(&dest)[N_DEST], const T(&source)[N_SOURCE], int n = N_SOURCE) {
  if (n > N_SOURCE) {
    n = N_SOURCE;
  }
  if (n > N_DEST) {
    n = N_DEST;
  }
  for (int i = 0; i < n; i++) {
    dest[i] = source[i];
  }
}

template <typename T, unsigned N>
inline void pushElem(T(&list)[N], int &n, T elem) {
  if (unsigned(n) < N - 1) {
    list[n] = elem;
    n++;
  }
}

template <typename T, unsigned N>
inline T removeElem(T(&list)[N], int &n, unsigned inx) {
  if (unsigned(n) < N) {
    T temp = list[inx];
    for (unsigned i = inx; i < n - 1; i++) {
      list[i] = list[i+1];
    }
    n--;
    return temp;
  }
  return T();
}
