# Git Cheatsheet

## push

* Force push

`git push --force` or `git push origin +branch_name`

## proxy

```
git config --global http.proxy http://proxyuser:proxypwd@proxy.server.com:8080
git config --system http.sslcainfo /bin/curl-ca-bundle.crt // Resolve CA issue 1: Set local CA cert
git config --global http.sslVerify false // Resolve CA issue 2: bypass
```
