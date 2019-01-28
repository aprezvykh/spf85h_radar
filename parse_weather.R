#!/usr/bin/Rscript
library(owmr)
Sys.setenv(OWM_API_KEY = "0f7aa48ec57b549e0fddbd311a5f2f46")
curr <- get_current(city = "Obninsk",units = "metric")
curr.temp <- paste("Temp: ", as.numeric(curr$main$temp), sep = "")
curr.hum <- paste("Hum: ", curr$main$humidity, sep = "")
curr.press <- paste("Press: ", curr$main$pressure, sep = "")
curr.windspeed <- paste("Wind speed: ", curr$wind$speed, sep = "")
curr.winddirection <- paste("Wind direction: ", curr$wind$deg, sep = "")
curr.vis <- paste("Visibility: ", curr$visibility,sep = "")
curr.clouds <- paste("Clouds: ", curr$clouds$all)
df <- data.frame(curr.temp,
           curr.hum,
           curr.press,
           curr.windspeed,
           curr.winddirection,
           curr.vis,
           curr.clouds)

df <- t(df)
write.table(file = "~/curr_weather.tsv",x = df,quote = F,row.names = F,col.names = F)

png("bknd.png")
plot(seq(1:10), seq(1:10))
dev.off()
