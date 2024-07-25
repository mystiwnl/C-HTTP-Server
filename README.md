<h1 align="center">Welcome to C-HTTP-Server 👋</h1>
<p>
  <img alt="Version" src="https://img.shields.io/badge/Version-0.2-blue.svg?cacheSeconds=2592000" />
</p>

>  A basic HTTP server that handles incoming client connections, and serves either a default response or specific files based on the client's request.

## Updates
```sh
0.2/ Added multi-threading to handel multiple connections and the ability to find any file that exists, not only the one in the code.


```

## Usage

```sh
Tested with curl and wget
```

## Run tests

```sh
wget localhost:1441, curl localhost:1441, or with a specific html file request, it also handles a not found 404 response
nc localhost:1441 , GET /hello.html(Also works with any other file, if it exists) HTTP/1.1
```

## Author

👤 **Matija Krivokuca**

* Github: [@mystiwnl](https://github.com/mystiwnl)
* LinkedIn: [@Matija Krivokuca](https://linkedin.com/in/matija-krivokuca-16287829a/)


