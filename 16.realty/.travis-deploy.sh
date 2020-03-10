#!/bin/sh
curl \
   -T realty-0.0.$TRAVIS_BUILD_NUMBER-Linux.deb \
   -ualekseysyryh:$BINTRAY_API_KEY \
   "https://api.bintray.com/content/alekseysyryh/otus-cpp2/homework/$TRAVIS_BUILD_NUMBER/realty-0.0.$TRAVIS_BUILD_NUMBER-Linux.deb;deb_distribution=xenial;deb_component=main;deb_architecture=amd64;publish=1"

body='{
"request": {
"branch":"master"
}}'

curl -s -X POST \
   -H "Content-Type: application/json" \
   -H "Accept: application/json" \
   -H "Travis-API-Version: 3" \
   -H "Authorization: token $TRAVIS_API_TOKEN" \
   -d "$body" \
https://api.travis-ci.org/repo/AlekseySyryh%2Fotuscpp_docker/requests