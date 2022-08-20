# DidgeridooMH's Load Balance Server

## Description and Goals

This a simple gateway load balancing server. You place this at the front of
your network and you can scale to however many servers you want. It uses a
round robin polic for load balancing and does simple HTTP message checking to
parse messages. The filter architecture allows for any layer to be placed into
the pipeline such as a TLS offloader, firewall security, parameter checkers and
more.

- [x] HTTP Message Relaying
- [x] Round Robin Endpoint Switching
- [ ] TLS Offloading
- [ ] API Validation
- [ ] AI Injection Detection

## Building

DLBS uses Meson for its build system and its recommended that Ninja is used for
the build backend, however Make will work in a pinch. Currently, all code is for
UNIX only (Mac/Linux), so sorry Windows users.

```sh
meson builddir
meson compile -C builddir
```

## Configuration

DLBS uses JSON for its configuration files. There is a table of keys and data types
below. If no config file is specified, `/etc/dlbs.json` is used.

```sh
dlbs [config]
```

| Key         | Type              |
| ----------- | ----------------- |
| `endpoints` | `Array<Endpoint>` |

### Endpoint

| Key       | Type     | Description                       |
| --------- | -------- | --------------------------------- |
| `address` | `string` | IP address of the endpoint node.  |
| `port`    | `number` | Port number of the endpoint node. |

