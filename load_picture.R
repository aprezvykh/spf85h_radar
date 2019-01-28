#!/usr/bin/Rscript
library(ggplot2)
big.df <- data.frame()
for(f in seq(1:60)){
    total.ram <- as.numeric(system("free -m | sed -n 2p | awk '{print $2}'",intern = T))
    used.ram <- as.numeric(system("free -m | sed -n 2p | awk '{print $3}'", intern = T))
    perc.ram <- 100*(used.ram / total.ram)
    perc.cpu <- as.numeric(system("grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage}'",intern = T))
    df <- data.frame(perc.cpu, perc.ram,f)
    big.df <- rbind(df, big.df)
    Sys.sleep(1)
}

names(big.df) <- c("cpu", "ram", "sec")
g <- ggplot(data=big.df) + geom_point(aes(x = sec, y = cpu), color = "red") + 
  geom_line(aes(x = sec, y = cpu), color = "red") + 
  geom_point(aes(x = sec, y = ram), color = "blue") + 
  geom_line(aes(x = sec, y = ram), color = "blue") + 
  theme_bw()
png("~/bgnd.png",width = 800,height = 600,units = "px")
g
dev.off()
