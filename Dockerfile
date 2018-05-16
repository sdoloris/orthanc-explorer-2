# usage
# cd into the folder with the Dockerfile
# docker build -t montefiore/orthanc-oe2 .
#
# to test it:
# docker run --rm -p 8042:8042 montefiore/orthanc-oe2
# to "debug it"
# docker run --rm -it --entrypoint=bash -p 8042:8042 montefiore/orthanc-oe2
# and once inside the container:
#   ./docker-entrypoint.sh /etc/orthanc
#
# to persist data from one run to the other
# docker run --rm  --volume /home/my-user/orthanc-db:/var/lib/orthanc/db -p 8042:8042 montefiore/orthanc-oe2

# once the image is ready, you can push it to Dockerhub
# docker push montefiore/orthanc-oe2



# build frontend
FROM trion/ng-cli as frontend-builder

RUN mkdir -p /home/node/frontend/explorer2
COPY src/frontend/explorer2/package.json /home/node/frontend/explorer2

RUN ls -al /home/node/frontend/explorer2
WORKDIR /home/node/frontend/explorer2
USER root
RUN npm i npm@latest -g
RUN npm install
RUN  mkdir -p /home/node/frontend/staticRessource

COPY src/frontend /home/node/frontend
COPY src/frontend/explorer2/.angular-cli.json /home/node/frontend/explorer2
RUN ng build



# build C++ code
FROM ubuntu:16.04 as backend-builder

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install wget build-essential unzip cmake uuid-dev && apt-get clean && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get -y install python

RUN mkdir /backend
RUN mkdir -p /frontend/staticRessource
RUN mkdir /build

COPY src/backend /backend
COPY --from=frontend-builder /home/node/frontend/staticRessource /frontend/staticRessource

WORKDIR /build
RUN cmake /backend
RUN make -j 5

# generate an Orthanc image with OE2 plugin
FROM osimis/orthanc as orthanc

RUN mkdir -p /var/lib/orthanc/db/
COPY --from=backend-builder /build/libOE2.so /usr/share/orthanc/plugins
