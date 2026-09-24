# REPORT.md — Operating Systems PA-01: libmyutils

**Name:** Muhammad Aneeq-uz-Zaman
**Roll No:** BSDSF24A033
**Repository:** https://github.com/Aneeq-uz-Zaman/BSDSF24A033-OS-A01

---

## Part 2: Multi-file Project (Direct Compilation)

### Q1. Explain the linking rule `$(TARGET): $(OBJECTS)`. How does it differ from a rule that links against a library?

This rule tells `make` that the final executable (`$(TARGET)`) depends on every object file listed in `$(OBJECTS)`. If any of those `.o` files is newer than the executable (or the executable doesn't exist yet), `make` runs the recipe, which calls the compiler as a linker on the full list: `gcc main.o mystrfunctions.o myfilefunctions.o -o client`. In our own Makefile (no macros used) the same rule is written out literally as `../bin/client: ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o`.

This is **direct linking**: the linker pulls the actual machine code out of every listed object file and merges it into one self-contained executable. Every function's code physically lives inside the final binary.

Linking against a library works differently. Instead of listing every object file the program needs, the rule references a pre-built library and uses the `-L` (library search path) and `-l` (library name) flags, e.g. `gcc main.o -L../lib -lmyutils -o client`. The build no longer needs to know which individual `.o` files make up `libmyutils` — that detail is hidden inside the library file. This decouples the application from the library's internals: the library can be rebuilt independently, and several different programs can link against the same compiled library without ever recompiling its source.

### Q2. What is a git tag and why is it useful? What is the difference between a lightweight and an annotated tag?

A git tag is a fixed, named pointer to one specific commit. Unlike a branch, a tag never moves as new commits are added — it permanently marks "this exact commit is version X," which makes it easy to check out, compare against, or build a release from that exact point in history later.

- **Lightweight tag** (`git tag v0.1.1-multifile`) is just a name pointing straight at a commit hash — no extra data, similar to a branch reference that can't move.
- **Annotated tag** (`git tag -a v0.1.1-multifile -m "message"`) is a full object stored in git's database: it records the tagger's name, email, date, and a message, and can be GPG-signed. GitHub Releases are built on top of annotated tags, which is why the assignment specifically asks for one.

### Q3. What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries to it?

A GitHub Release packages a tagged commit into something end users can actually consume: a title, a description/changelog, and (optionally) downloadable files, all shown on a dedicated page. It turns a point in the commit history into a distributable version of the software.

Attaching a compiled binary (like `bin/client`) matters because it lets someone download and run the program immediately — without cloning the repository, installing a compiler, or running `make` themselves. GitHub already auto-generates a source-code zip/tarball for every tag, but that's just the source; producing and attaching the actual compiled artifact is the maintainer's job, and it's what separates "here's the code as of this tag" from "here's the program you can run right now."

---

## Part 3: Static Library

### Q1. Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?

In Part 2, the final target linked directly from all three object files:
```
../bin/client: ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	gcc ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o -o ../bin/client
```

In Part 3, an extra rule sits in between: instead of `mystrfunctions.o` and `myfilefunctions.o` going straight into the final link, they first get archived into `lib/libmyutils.a`:
```
../lib/libmyutils.a: ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ar rc ../lib/libmyutils.a ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ranlib ../lib/libmyutils.a
```
and the final executable (`client_static`) now depends on `main.o` plus the library archive, linking against it with `-L`/`-l` instead of listing raw object files:
```
../bin/client_static: ../obj/main.o ../lib/libmyutils.a
	gcc ../obj/main.o -L../lib -lmyutils -o ../bin/client_static
```
So the key differences: (1) a new intermediate target that builds `libmyutils.a` using `ar`/`ranlib`, and (2) the final link rule's recipe switches from listing every object file individually to `-L../lib -lmyutils`, which tells the linker "search `lib/` for a library named `myutils` and pull in whatever symbols are actually needed from it."

### Q2. What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?

`ar` (archiver) bundles multiple object files into a single archive file (`.a`) without compressing them — a static library is really just a collection of `.o` files glued together with an index. Running `ar -t lib/libmyutils.a` confirms both object files landed inside it:
```
mystrfunctions.o
myfilefunctions.o
```

`ranlib` builds a symbol index inside that archive — a lookup table mapping each function name (e.g. `mystrlen`) to the specific `.o` member that defines it. Without this index, the linker would have to scan every object file in the archive one by one to find a symbol; with it, the linker can jump straight to the right member. Some versions of `ar` (the `s` flag, i.e. `ar rcs`) build this index automatically, but running `ranlib` explicitly makes the two separate responsibilities — archiving vs. indexing — visible.

### Q3. When you run `nm` on `client_static`, are the symbols for functions like `mystrlen` present? What does this tell you about static linking?

Yes. Running `nm bin/client_static | grep mystrlen` shows:
```
00000000000016d4 T mystrlen
```
The `T` means the symbol is defined in the executable's own text (code) section — the actual machine code for `mystrlen` has been copied straight into `client_static`. The same is true for every other library function (`mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, `mygrep` all show up with `nm bin/client_static`).

This confirms how static linking works: at link time, the linker pulls whichever object files (from `libmyutils.a`) actually define the symbols the program uses, and physically copies their code into the final binary. The resulting executable is self-contained — it doesn't need `libmyutils.a` to exist anymore at runtime, since the library's code no longer lives in a separate file, it lives inside `client_static` itself.

---

## Part 4: Dynamic Library

### Q1. What is Position-Independent Code (`-fPIC`) and why is it required for shared libraries?

Position-Independent Code is machine code that works correctly no matter what memory address it gets loaded at, because it never hardcodes absolute addresses — it accesses data and functions using relative offsets instead. This is compiled with `gcc -fPIC -c mystrfunctions.c -o ../obj/mystrfunctions_pic.o` (a separate object file from the one used in the static build).

It's required for shared libraries because a `.so` is loaded once into memory and then shared by every process that uses it — but each process can end up mapping that library at a different address in its own address space. A statically-linked program doesn't have this problem because its code is baked into one fixed executable at link time, but a shared library has to work identically regardless of where the loader happens to place it. Without `-fPIC`, the library's code would assume a fixed address and break for any process that loads it somewhere else.

### Q2. Explain the difference in file size between the static and dynamic clients. Why does this difference exist?

```
16840 bin/client_static
16456 bin/client_dynamic
```
`client_dynamic` is smaller than `client_static` by 384 bytes. The reason: `client_static` has the actual machine code for `mystrlen`, `mystrcpy`, `wordCount`, `mygrep`, etc. copied directly into it (confirmed with `nm` in Part 3), while `client_dynamic` only stores a reference saying "look up these symbols in `libmyutils.so` at runtime" — the real code stays in the separate `.so` file (16104 bytes) and isn't duplicated into the executable.

The absolute difference here is small only because `libmyutils` itself is a tiny library with a handful of short functions — most of both executables' size is fixed overhead (ELF headers, C runtime startup code, symbol tables) that exists either way. With a larger, more realistic library, the same effect would produce a much bigger gap, since every additional function would add its full size to a static executable but add nothing to a dynamic one.

### Q3. What is `LD_LIBRARY_PATH`? Why was it necessary, and what does this tell you about the responsibilities of the dynamic loader?

`LD_LIBRARY_PATH` is an environment variable that tells the dynamic loader (`ld.so`) extra directories to search when a program needs to load a shared library at startup. Running `./bin/client_dynamic` without it fails immediately:
```
./bin/client_dynamic: error while loading shared libraries: libmyutils.so: cannot open shared object file: No such file or directory
```
This happens because `libmyutils.so` isn't installed anywhere the loader checks by default (like `/usr/lib`) — it only exists in this project's own `lib/` folder, which isn't on the loader's standard search path. Setting it fixes that:
```bash
export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
```
After that, `./bin/client_dynamic` runs correctly, and `ldd bin/client_dynamic` confirms exactly where it resolved the library from:
```
libmyutils.so => /home/aneeq/OS/BSDSF24A033-OS-A01/lib/libmyutils.so
```
This shows that unlike static linking (where everything needed is already inside the executable), dynamic linking defers finding and loading library code until the program actually starts running. The dynamic loader is responsible for locating every shared library a program depends on at launch time, and if it can't find one, the program can't even start — the dependency is resolved at runtime, not at compile/link time.

---

## Part 5: Man Pages & Installation

_(No report questions specified for this part in the assignment; notes on the install process below.)_

Man pages were written in `man/man3/` for each library function (`mystrlen`, `mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, `mygrep`) and in `man/man1/client.1` for the driver program itself, each with `.TH`, `.SH NAME`, `.SH SYNOPSIS`, `.SH DESCRIPTION`, and `.SH AUTHOR` sections. They were previewed locally with `man -l man/man3/mystrlen.3` before installing.

The `install` target (added to both the root `Makefile` and `src/Makefile`) copies `bin/client_static` to `/usr/local/bin/client` and all man pages to `/usr/local/share/man/man{1,3}`. `client_static` was installed rather than `client_dynamic`, since a statically-linked binary is self-contained — installing the dynamic version system-wide would additionally require installing `libmyutils.so` to a system library directory and running `ldconfig`, which the assignment doesn't ask for. After `sudo make install`, `client` runs from any directory and `man client` / `man mystrlen` work without extra configuration, since `/usr/local/bin` and `/usr/local/share/man` are already on the default `PATH`/`MANPATH`.

---

## Part 6: Final Notes

All feature branches were completed, tagged, and merged into `main` in sequence:

| Branch | Tag | Release |
|---|---|---|
| `multifile-build` | `v0.1.1-multifile` | Version 0.1.0: Multi-file Build |
| `static-build` | `v0.2.1-static` | Version 0.2.1: Static Library Build |
| `dynamic-build` | `v0.3.1-dynamic` | Version 0.3.1: Dynamic Library Build |
| `man-pages` | `v0.4.1-final` | Version 0.4.1: Final Build |

Each branch was merged into `main` before starting the next feature (per the assignment's stated sequencing), and all branches remain pushed to the remote alongside `main` for the complete development history.

Building this project end to end — direct multi-file compilation, then a static library, then a dynamic library, then packaging with man pages and an install target — made the tradeoffs between the approaches concrete rather than theoretical: the same six functions moved from being copied directly into one executable, to being archived and still copied in at static-link time, to finally being loaded from a separate `.so` file at runtime and resolved by the dynamic loader via `LD_LIBRARY_PATH`. Inspecting the results at each stage with `nm`, `readelf`, `ldd`, and `ls -lh` turned "static vs. dynamic linking" from a textbook distinction into something directly observable in the built artifacts.
