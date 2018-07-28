#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double r;
	double i;
}my_complex
;

//检查a是否为2的整数次方数
#define NOT2POW(a) (((a)-1)&(a)||(a)<=0)
//pi
#define MYPI 3.14159265358979323846

my_complex* fft(const my_complex* x, unsigned int len) {
	unsigned int ex = 0, t = len;
	unsigned int i, j, k;
	my_complex *y;
	double tr, ti, rr, ri, yr, yi;

	if (NOT2POW(len)) return NULL;        //如果失败，返回空指针
	for (; !(t & 1); t >>= 1) ex++;        //len应该等于2的ex次方

	y = (my_complex*)malloc(len * sizeof(my_complex));
	if (!y) return NULL;

	//变址计算，库里-图基算法
	for (i = 0; i<len; i++) {
		k = i;
		j = 0;
		t = ex;
		while ((t--)>0) {
			j <<= 1;
			j |= k & 1;
			k >>= 1;
		}
		if (j >= i) {
			y[i] = x[j];
			y[j] = x[i];
		}
	}

	//用变址后的y向量进行计算
	for (i = 0; i<ex; i++) {
		t = 1 << i;
		for (j = 0; j<len; j += t << 1) {
			for (k = 0; k<t; k++) {
				ti = -MYPI * k / t;
				rr = cos(ti);
				ri = sin(ti);

				tr = y[j + k + t].r;
				ti = y[j + k + t].i;

				yr = rr * tr - ri * ti;
				yi = rr * ti + ri * tr;

				tr = y[j + k].r;
				ti = y[j + k].i;

				y[j + k].r = tr + yr;
				y[j + k].i = ti + yi;
				y[j + k + t].r = tr - yr;
				y[j + k + t].i = ti - yi;
			}
		}
	}

	return y;
}

//以下为测试
int main()
{
	int i, DATA_LEN;
	my_complex *x, *y;

	printf("基二FFT测试\n输入生成序列长度:");
	scanf("%d", &DATA_LEN);
	x = (my_complex*)malloc(DATA_LEN * sizeof(my_complex));

	for (i = 0; i<DATA_LEN; i++) {
		x[i].r = i;
		x[i].i = i - 1;
	}

	printf("处理前...\n实部\t\t虚部\n");
	for (i = 0; i<DATA_LEN; i++)
		printf("%lf\t%lf\n", x[i].r, x[i].i);


	y = fft(x, DATA_LEN);
	if (!y) {
		printf("序列长度不为2的整数次方!\n");
		return 0;
	}

	printf("处理后...\n实部\t\t虚部\n");
	for (i = 0; i<DATA_LEN; i++)
		printf("%lf\t%lf\n", y[i].r, y[i].i);

	free(y);
	free(x);

	return 0;
}
