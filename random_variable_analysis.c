#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N_SAMPLES 10000
#define PI 3.14159265358979323846
#define BINS 50

// 全局变量存储数据
double bernoulli_samples[N_SAMPLES];
double normal_samples[N_SAMPLES];
double combined_samples[N_SAMPLES];
int histogram[BINS];
double bin_centers[BINS];

// 函数声明
void generate_bernoulli_samples(int n);
void generate_normal_samples(int n);
void combine_samples(int n);
void calculate_statistics(double *data, int n, double *mean, double *std, double *var);
void create_histogram(double *data, int n, int bins);
void print_statistics(double mean, double std, double var, double min, double max);
void print_histogram(int bins);
void theoretical_analysis(void);
double normal_pdf(double x);
double box_muller(void);

int main() {
    printf("随机变量组合分析程序 (C语言版本)\n");
    printf("==================================================\n");
    
    // 设置随机种子
    srand((unsigned int)time(NULL));
    
    // 1. 生成贝努利分布随机变量(-1,1)
    printf("1. 生成贝努利分布随机变量(-1,1)...\n");
    generate_bernoulli_samples(N_SAMPLES);
    printf("生成了 %d 个样本\n", N_SAMPLES);
    
    double bernoulli_mean, bernoulli_std, bernoulli_var;
    calculate_statistics(bernoulli_samples, N_SAMPLES, &bernoulli_mean, &bernoulli_std, &bernoulli_var);
    printf("贝努利变量统计: 均值=%.4f, 标准差=%.4f\n", bernoulli_mean, bernoulli_std);
    
    // 2. 生成标准正态分布随机变量
    printf("\n2. 生成标准正态分布N(0,1)随机变量...\n");
    generate_normal_samples(N_SAMPLES);
    
    double normal_mean, normal_std, normal_var;
    calculate_statistics(normal_samples, N_SAMPLES, &normal_mean, &normal_std, &normal_var);
    printf("正态变量统计: 均值=%.4f, 标准差=%.4f\n", normal_mean, normal_std);
    
    // 3. 组合两个随机变量
    printf("\n3. 组合两个随机变量...\n");
    combine_samples(N_SAMPLES);
    
    // 4. 分析概率分布
    printf("\n4. 分析组合变量的概率分布...\n");
    double mean_val, std_val, var_val;
    calculate_statistics(combined_samples, N_SAMPLES, &mean_val, &std_val, &var_val);
    
    double min_val = combined_samples[0];
    double max_val = combined_samples[0];
    for (int i = 1; i < N_SAMPLES; i++) {
        if (combined_samples[i] < min_val) min_val = combined_samples[i];
        if (combined_samples[i] > max_val) max_val = combined_samples[i];
    }
    
    print_statistics(mean_val, std_val, var_val, min_val, max_val);
    
    // 5. 理论分析
    theoretical_analysis();
    
    // 6. 创建直方图
    printf("\n5. 创建概率分布直方图...\n");
    create_histogram(combined_samples, N_SAMPLES, BINS);
    print_histogram(BINS);
    
    // 7. 验证理论结果
    printf("\n验证理论结果:\n");
    printf("理论均值: 0, 实际均值: %.4f\n", mean_val);
    printf("理论方差: 2, 实际方差: %.4f\n", var_val);
    printf("理论标准差: √2 ≈ 1.414, 实际标准差: %.4f\n", std_val);
    
    return 0;
}

void generate_bernoulli_samples(int n) {
    for (int i = 0; i < n; i++) {
        // 生成0或1的贝努利分布
        int bernoulli = rand() % 2;
        // 映射为-1或1
        bernoulli_samples[i] = 2 * bernoulli - 1;
    }
}

void generate_normal_samples(int n) {
    for (int i = 0; i < n; i++) {
        normal_samples[i] = box_muller();
    }
}

void combine_samples(int n) {
    for (int i = 0; i < n; i++) {
        combined_samples[i] = bernoulli_samples[i] + normal_samples[i];
    }
}

void calculate_statistics(double *data, int n, double *mean, double *std, double *var) {
    // 计算均值
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    *mean = sum / n;
    
    // 计算方差
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - *mean;
        sum_sq_diff += diff * diff;
    }
    *var = sum_sq_diff / n;
    *std = sqrt(*var);
}

void create_histogram(double *data, int n, int bins) {
    // 找到数据范围
    double min_val = data[0];
    double max_val = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
    }
    
    // 计算bin宽度和中心
    double bin_width = (max_val - min_val) / bins;
    for (int i = 0; i < bins; i++) {
        bin_centers[i] = min_val + (i + 0.5) * bin_width;
        histogram[i] = 0;
    }
    
    // 填充直方图
    for (int i = 0; i < n; i++) {
        int bin_index = (int)((data[i] - min_val) / bin_width);
        if (bin_index >= bins) bin_index = bins - 1;
        histogram[bin_index]++;
    }
}

void print_statistics(double mean, double std, double var, double min, double max) {
    printf("组合变量的统计信息:\n");
    printf("均值: %.4f\n", mean);
    printf("标准差: %.4f\n", std);
    printf("方差: %.4f\n", var);
    printf("最小值: %.4f\n", min);
    printf("最大值: %.4f\n", max);
}

void print_histogram(int bins) {
    printf("\n直方图 (概率分布):\n");
    printf("区间中心\t频数\t相对频率\t图形表示\n");
    printf("--------\t----\t--------\t--------\n");
    
    int max_freq = 0;
    for (int i = 0; i < bins; i++) {
        if (histogram[i] > max_freq) max_freq = histogram[i];
    }
    
    for (int i = 0; i < bins; i++) {
        double relative_freq = (double)histogram[i] / N_SAMPLES;
        printf("%.2f\t\t%d\t%.4f\t\t", bin_centers[i], histogram[i], relative_freq);
        
        // 简单的文本图形表示
        int bar_length = (int)((double)histogram[i] / max_freq * 50);
        for (int j = 0; j < bar_length; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void theoretical_analysis(void) {
    printf("\n理论分析:\n");
    printf("设 X ~ Bernoulli(-1,1), Y ~ N(0,1)\n");
    printf("则 Z = X + Y 的分布为:\n");
    printf("E[Z] = E[X] + E[Y] = 0 + 0 = 0\n");
    printf("Var[Z] = Var[X] + Var[Y] = 1 + 1 = 2\n");
    printf("因此 Z 的分布是两个正态分布的混合:\n");
    printf("Z ~ 0.5 * N(-1,1) + 0.5 * N(1,1)\n");
    printf("即 Z 的密度函数为:\n");
    printf("f(z) = 0.5 * φ(z+1) + 0.5 * φ(z-1)\n");
    printf("其中 φ 是标准正态分布的密度函数\n");
}

double normal_pdf(double x) {
    return (1.0 / sqrt(2 * PI)) * exp(-0.5 * x * x);
}

double box_muller(void) {
    // Box-Muller变换生成标准正态分布
    static double z0, z1;
    static int generate = 0;
    
    if (generate == 0) {
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;
        
        z0 = sqrt(-2 * log(u1)) * cos(2 * PI * u2);
        z1 = sqrt(-2 * log(u1)) * sin(2 * PI * u2);
        
        generate = 1;
        return z0;
    } else {
        generate = 0;
        return z1;
    }
}
