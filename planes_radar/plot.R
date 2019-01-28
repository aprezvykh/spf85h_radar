#!/usr/bin/Rscript
library(ggmap)
library("methods")
library("ggplot2")
library("BMS")
deg2rad <- function(deg) {(deg * pi) / (180)}
invert <- function(x) rgb(t(255-col2rgb(x))/255)
theme_blank <- function(...) {
  ret <- theme_bw(...)
  ret$line <- element_blank()
  ret$rect <- element_blank()
  ret$strip.text <- element_blank()
  ret$axis.text <- element_blank()
  ret$plot.title <- element_blank()
  ret$axis.title <- element_blank()
  ret$panel.spacing <- unit(1, "lines")
  ret$plot.margin <- structure(c(-1, -1, -5, -9), unit = "lines", valid.unit = 3L, class = "unit")
  ret$plot.title = NULL
  ret$plot.background = element_rect(fill = "black")
  ret$strip.background = element_rect(fill= "black")
  ret
}

theme(plot.margin = )
dir <- "~/planes_radar/"
setwd(dir)
is.empty <- system("cat test.log",intern = T)

if(identical(is.empty, character(0))){
  png("~/bgnd.png",width = 800,height = 600,units = "px",bg = "black")
  plot(c(0, 1), c(0, 1), ann = F, bty = 'n', type = 'n', xaxt = 'n', yaxt = 'n')
  text(x = 0.5, y = 0.5, paste("No planes are currently detactable"), 
       cex = 4, col = "green")
  dev.off()
}


dat <- read.table("test.log",header = F,sep = ",")
names(dat) <- c("type", "n", "X1", "X2", "flight",
                "X3", "X4", "X5", "X6", "X7", "X8",
                "alt", "speed", "heading", "lat", "lon",
                "vertical", "X9", "X10", "X11", "X12", "X13")
dat <- data.frame(dat$flight, dat$alt, dat$speed, dat$heading,
                  dat$lat, dat$lon, dat$vertical)

big.means <- data.frame()
for(f in unique(dat$dat.flight)){
  print(f)
  df <- dat[dat$dat.flight == f,]
  df.alt <- mean(na.omit(df$dat.alt))
  df.speed <- mean(na.omit(df$dat.speed))
  df.hdg <- mean(na.omit(df$dat.heading))
  df.lat <- mean(na.omit(df$dat.lat))
  if(is.nan(unique(df.lat))){
    next
  }
  df.lon <- mean(na.omit(df$dat.lon))
  if(is.nan(unique(df.lon))){
    next
  }
  df.vert <- mean(na.omit(df$dat.vertical))
  means.df <- data.frame(f, df.alt, df.speed, df.hdg, df.lat, df.lon, df.vert)
  big.means <- rbind(means.df, big.means)
}

###here is default coords
#coords <- c(left = 35.812683, bottom = 54.557730, right = 38.836670,top = 55.624895)
#coords <- c(left = 35.212683, bottom = 54.157730, right = 38.836670,top = 55.624895)
#map <- get_stamenmap(bbox = coords, maptype = "toner")
#m_inv <- as.raster(apply(map, 2, invert))
#class(m_inv) <- class(map)
#attr(m_inv, "bb") <- attr(map, "bb")
#saveRDS(m_inv, "map.RDS")

map <- readRDS("map.RDS")
names(big.means) <- c("id", "alt", "speed", "hdg", "lat", "lon", "vert")


write.table(big.means, "archive.log", append = T, col.names = F,row.names = F,quote = F,sep = "\t")
me <- data.frame(lat = 55.117038, lon = 36.597082)
lims <- read.table("archive.log")
names(lims) <- c("id", "alt", "speed", "hdg", "lat", "lon", "vert")
png("~/bgnd.png",width = 800,height = 600,units = "px",bg = "black")
ggmap(map) + geom_point(data = big.means, aes(x = lon, y = lat), color = "green") + 
                               geom_text(data = big.means, aes(x = lon, y = lat, label = paste(id, "\n", 
                                                                             "Alt: ", round(alt,0), "\n",
                                                                             "Speed: ", round(speed,0), "\n",
                                                                             "Vert: ", round(vert,0),
                                                                             sep = "")),
                                                                             hjust = 1.1, vjust = 1.1,size = 3,color = "green") + 
                              geom_spoke(data = big.means, aes(x = lon, y = lat, angle = hdg),radius = 0.08, color = "green") + 
			      theme_blank() + 
                              geom_density2d(data = lims, aes(x = lon, y = lat),color = "blue", alpha = 0.4) + 
                              geom_point(data=me, aes(x = lon,y = lat), color = "red")

dev.off()
