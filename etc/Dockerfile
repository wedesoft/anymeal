FROM debian:sid
LABEL maintainer="Jan Wedekind <jan@wedesoft.de>"
RUN echo "deb http://httpredir.debian.org/debian unstable main" > /etc/apt/sources.list
RUN apt-get update  # Forced update Fri 20 Sep 15:16:57 BST 2024
RUN apt-get -q -y dist-upgrade
RUN apt-get install -y xauth
ADD anymeal_1.33-1_amd64.deb anymeal_1.33-1_amd64.deb
RUN apt install -y ./anymeal_1.33-1_amd64.deb
# get string with xauth list
RUN xauth add wedetux/unix:1  MIT-MAGIC-COOKIE-1  a2abfc88618447f209e71a5dc1d23ef6
CMD ["anymeal"]
