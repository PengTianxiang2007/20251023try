import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
import seaborn as sns

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

def generate_bernoulli_like_variable(n_samples=10000):
    """
    生成服从贝努利二项分布的随机变量(-1,1)
    这里使用等概率生成-1和1
    """
    # 生成0和1的贝努利分布
    bernoulli = np.random.binomial(1, 0.5, n_samples)
    # 将0映射为-1，1保持为1
    bernoulli_like = 2 * bernoulli - 1
    return bernoulli_like

def generate_standard_normal(n_samples=10000):
    """
    生成标准正态分布N(0,1)的随机变量
    """
    return np.random.normal(0, 1, n_samples)

def combine_variables(bernoulli_like, normal):
    """
    将贝努利分布变量和正态分布变量相加
    """
    return bernoulli_like + normal

def analyze_distribution(combined_variable):
    """
    分析组合变量的概率分布
    """
    # 基本统计信息
    mean_val = np.mean(combined_variable)
    std_val = np.std(combined_variable)
    var_val = np.var(combined_variable)
    
    print(f"组合变量的统计信息:")
    print(f"均值: {mean_val:.4f}")
    print(f"标准差: {std_val:.4f}")
    print(f"方差: {var_val:.4f}")
    print(f"最小值: {np.min(combined_variable):.4f}")
    print(f"最大值: {np.max(combined_variable):.4f}")
    
    return mean_val, std_val, var_val

def plot_distribution(combined_variable, n_samples):
    """
    绘制概率分布图
    """
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    
    # 1. 直方图
    axes[0, 0].hist(combined_variable, bins=50, density=True, alpha=0.7, color='skyblue', edgecolor='black')
    axes[0, 0].set_title('组合变量的直方图')
    axes[0, 0].set_xlabel('数值')
    axes[0, 0].set_ylabel('概率密度')
    axes[0, 0].grid(True, alpha=0.3)
    
    # 2. 密度图
    axes[0, 1].hist(combined_variable, bins=50, density=True, alpha=0.7, color='lightcoral', edgecolor='black')
    # 添加核密度估计
    from scipy.stats import gaussian_kde
    kde = gaussian_kde(combined_variable)
    x_range = np.linspace(combined_variable.min(), combined_variable.max(), 200)
    axes[0, 1].plot(x_range, kde(x_range), 'r-', linewidth=2, label='核密度估计')
    axes[0, 1].set_title('概率密度图')
    axes[0, 1].set_xlabel('数值')
    axes[0, 1].set_ylabel('概率密度')
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)
    
    # 3. Q-Q图（检验正态性）
    stats.probplot(combined_variable, dist="norm", plot=axes[1, 0])
    axes[1, 0].set_title('Q-Q图（正态性检验）')
    axes[1, 0].grid(True, alpha=0.3)
    
    # 4. 箱线图
    axes[1, 1].boxplot(combined_variable, vert=True)
    axes[1, 1].set_title('箱线图')
    axes[1, 1].set_ylabel('数值')
    axes[1, 1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()

def theoretical_analysis():
    """
    理论分析：组合变量的理论分布
    """
    print("\n理论分析:")
    print("设 X ~ Bernoulli(-1,1), Y ~ N(0,1)")
    print("则 Z = X + Y 的分布为:")
    print("E[Z] = E[X] + E[Y] = 0 + 0 = 0")
    print("Var[Z] = Var[X] + Var[Y] = 1 + 1 = 2")
    print("因此 Z 的分布是两个正态分布的混合:")
    print("Z ~ 0.5 * N(-1,1) + 0.5 * N(1,1)")
    print("即 Z 的密度函数为:")
    print("f(z) = 0.5 * φ(z+1) + 0.5 * φ(z-1)")
    print("其中 φ 是标准正态分布的密度函数")

def main():
    """
    主函数
    """
    print("随机变量组合分析程序")
    print("=" * 50)
    
    # 设置随机种子以便结果可重现
    np.random.seed(42)
    n_samples = 10000
    
    # 1. 生成贝努利分布随机变量(-1,1)
    print("1. 生成贝努利分布随机变量(-1,1)...")
    bernoulli_like = generate_bernoulli_like_variable(n_samples)
    print(f"生成了 {n_samples} 个样本")
    print(f"贝努利变量统计: 均值={np.mean(bernoulli_like):.4f}, 标准差={np.std(bernoulli_like):.4f}")
    
    # 2. 生成标准正态分布随机变量
    print("\n2. 生成标准正态分布N(0,1)随机变量...")
    normal_variable = generate_standard_normal(n_samples)
    print(f"正态变量统计: 均值={np.mean(normal_variable):.4f}, 标准差={np.std(normal_variable):.4f}")
    
    # 3. 组合两个随机变量
    print("\n3. 组合两个随机变量...")
    combined_variable = combine_variables(bernoulli_like, normal_variable)
    
    # 4. 分析概率分布
    print("\n4. 分析组合变量的概率分布...")
    mean_val, std_val, var_val = analyze_distribution(combined_variable)
    
    # 5. 理论分析
    theoretical_analysis()
    
    # 6. 绘制分布图
    print("\n5. 绘制概率分布图...")
    plot_distribution(combined_variable, n_samples)
    
    # 7. 验证理论结果
    print(f"\n验证理论结果:")
    print(f"理论均值: 0, 实际均值: {mean_val:.4f}")
    print(f"理论方差: 2, 实际方差: {var_val:.4f}")
    print(f"理论标准差: √2 ≈ 1.414, 实际标准差: {std_val:.4f}")

if __name__ == "__main__":
    main()
