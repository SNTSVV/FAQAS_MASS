
#method="pearson"
method="spearman"
#method="kendall"

print("FMC-FM")

x=c(12,1,3,6)
y=c(0.90,1,1,1)

cor.test(x, y, method=method)


print("MOC-FM")

x=c(12,1,3,6)
y=c(0.74,0.9565,0.8276,0.9125)

cor.test(x, y, method=method)

print("MOC-MO")

x=c(177,23,29,80)
y=c(0.74,0.9565,0.8276,0.9125)

cor.test(x, y, method=method)


print("MS-FM")


x=c(12,1,3,6)
y=c(0.45, 0.9545, 1, 0.3836)
cor.test(x, y, method=method)


print("MS-MO")

x=c(177,23,29,80)
y=c(0.45, 0.9545, 1, 0.3836)

cor.test(x, y, method=method)

print("MS-CMO")

x=c( 140, 23, 29, 80)
y=c(0.45, 0.9545, 1, 0.3836)

cor.test(x, y, method=method)
