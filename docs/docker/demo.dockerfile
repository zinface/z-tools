# Docker image of enterprise-manager-web
# VERSION 0.0.1
# Author: Jiang Xin

FROM java:8
MAINTAINER xxxx <xxxxx@tech-oled.com>
VOLUME /tmp
ADD enterprise-manager-web-0.0.1-SNAPSHOT.jar app.jar
RUN sh -c 'touch /app.jar'
ENV JAVA_OPTS=""
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-jar","/app.jar"]