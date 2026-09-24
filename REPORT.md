# REPORT.md — Operating Systems PA-01: libmyutils

**Name:** Muhammad Aneeq-uz-Zaman
**Roll No:** BSDSF24A033
**Repository:** https://github.com/Aneeq-uz-Zaman/BSDSF24A033-OS-A01

---

## Part 2: Multi-file Project (Direct Compilation)

### Q1. Explain the linking rule `$(TARGET): $(OBJECTS)`. How does it differ from a rule that links against a library?

This rule tells `make`: "to build the final program, you need these object files first." If any object file is newer than the program, `make` rebuilds it by running the compiler like this: `gcc main.o mystrfunctions.o myfilefunctions.o -o client`. In our own Makefile, this same rule looks like `../bin/client: ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o`.

This is called **direct linking** — the linker takes the real code out of every object file and copies it straight into the final program.

Linking against a library works differently. Instead of listing every object file, we just tell the linker where to look and what to use: `gcc main.o -L../lib -lmyutils -o client`. Here `-L` says "look in this folder" and `-l` says "use the library with this name." The program no longer needs to know exactly which `.o` files make up the library — it's hidden inside the library file. This makes it easy to update the library later, or reuse it in other programs, without recompiling everything.

### Q2. What is a git tag and why is it useful? What is the difference between a lightweight and an annotated tag?

A git tag is just a name we give to one specific commit, so we can find it again easily later — like a bookmark. Unlike a branch, a tag never moves. Once we tag a commit `v0.1.1-multifile`, that name always points to that exact commit, even after we keep making new commits elsewhere.

- **Lightweight tag** — just a name pointing to a commit. Nothing else attached.
- **Annotated tag** — also stores who made it, when, and a message, almost like its own tiny commit. GitHub Releases are built on top of annotated tags, which is why the assignment asks for one.

### Q3. What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries to it?

A GitHub Release takes one tag and turns it into something people can actually use — a title, a description, and (if we want) files attached, like our compiled program. It turns "a point in the code's history" into "a version someone can download."

Attaching the compiled program (`bin/client`) matters because now anyone can download and run it directly, without installing a compiler or typing any build commands. GitHub already gives people the source code automatically for every tag, but the actual ready-to-run program is something we have to build and upload ourselves.

---

## Part 3: Static Library

### Q1. Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?

In Part 2, the final program was built directly from all three object files:
```
../bin/client: ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	gcc ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o -o ../bin/client
```

In Part 3, there's one extra step. Instead of putting `mystrfunctions.o` and `myfilefunctions.o` straight into the final program, we first pack them into one library file, `lib/libmyutils.a`:
```
../lib/libmyutils.a: ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ar rc ../lib/libmyutils.a ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ranlib ../lib/libmyutils.a
```
Then the final program (`client_static`) only needs `main.o` plus that library file:
```
../bin/client_static: ../obj/main.o ../lib/libmyutils.a
	gcc ../obj/main.o -L../lib -lmyutils -o ../bin/client_static
```
So the two key differences are: (1) there's a new step that builds `libmyutils.a` using `ar` and `ranlib`, and (2) instead of listing every object file by name in the final link, we just say `-L../lib -lmyutils`, which means "look in the `lib` folder for a library called `myutils`."

### Q2. What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?

`ar` (short for "archiver") takes several object files and packs them into one file called a static library (`.a`). It doesn't compress anything — it just bundles the files together. Running `ar -t lib/libmyutils.a` proves both files really are inside:
```
mystrfunctions.o
myfilefunctions.o
```

`ranlib` adds a small index inside that library file — basically a table saying "the function `mystrlen` is inside this particular object file." Without this index, the linker would have to open and search every object file one by one to find a function. With the index, it can jump straight to the right one. (Some versions of `ar` can build this index automatically using the `s` flag, but we ran `ranlib` as its own separate step so it's easier to see what it actually does.)

### Q3. When you run `nm` on `client_static`, are the symbols for functions like `mystrlen` present? What does this tell you about static linking?

Yes. Running `nm bin/client_static | grep mystrlen` shows:
```
00000000000016d4 T mystrlen
```
The `T` means this function's real code is sitting inside the program itself. The same is true for every other function too (`mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, `mygrep`).

This shows exactly how static linking works: when the program is built, the linker copies the real code from the library straight into the final program. That's why `client_static` doesn't need `libmyutils.a` to exist anymore once it's built — everything it needs is already baked in.

---

## Part 4: Dynamic Library

### Q1. What is Position-Independent Code (`-fPIC`) and why is it required for shared libraries?

Position-Independent Code (PIC) is code that works correctly no matter where in memory it ends up, because it doesn't hardcode any fixed memory addresses. We compile it with `gcc -fPIC -c mystrfunctions.c -o ../obj/mystrfunctions_pic.o` — a separate object file from the one used for the static build.

Shared libraries need this because the same `.so` file can be loaded by many different programs at the same time, and each one might load it at a different spot in memory. A normal (static) program doesn't have this problem, since it's the only thing using its own code. But a shared library has to work correctly no matter where it lands — and that's only possible if the code never depends on a fixed address.

### Q2. Explain the difference in file size between the static and dynamic clients. Why does this difference exist?

```
16840 bytes  bin/client_static
16456 bytes  bin/client_dynamic
```
`client_dynamic` is 384 bytes smaller. The reason: `client_static` has the real code for every function copied inside it. `client_dynamic` just keeps a small note saying "look up this function in `libmyutils.so` when you run me" — the real code stays in that separate file (16104 bytes) instead of being duplicated into the program.

The difference looks small here only because our library is tiny, with just a few short functions. Most of the size in both programs actually comes from fixed overhead (program headers, startup code) that exists either way. If the library had many more functions, the static version would keep growing while the dynamic one would barely change.

### Q3. What is `LD_LIBRARY_PATH`? Why was it necessary, and what does this tell you about the responsibilities of the dynamic loader?

`LD_LIBRARY_PATH` is an environment variable that tells the program's loader extra places to look for shared library files when it starts up. Without it, running `./bin/client_dynamic` fails right away:
```
./bin/client_dynamic: error while loading shared libraries: libmyutils.so: cannot open shared object file: No such file or directory
```
That's because `libmyutils.so` only exists in our own project's `lib/` folder, which the loader doesn't check by default. Setting the variable fixes it:
```bash
export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
```
After that, the program runs fine, and `ldd bin/client_dynamic` shows exactly where it found the library:
```
libmyutils.so => /home/aneeq/OS/BSDSF24A033-OS-A01/lib/libmyutils.so
```
This shows a key difference from static linking: a dynamically-linked program doesn't actually carry the library's code inside it — it only finds and loads the library the moment it starts running. If the loader can't find it at that moment, the program can't even start. Static linking doesn't have this problem, because everything it needs is already built in.

---

## Part 5: Man Pages & Installation

_(No report questions specified for this part in the assignment; notes on the install process below.)_

Man pages were written in `man/man3/` for each library function (`mystrlen`, `mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, `mygrep`), and in `man/man1/client.1` for the driver program itself. Each page has a `.TH` (title), `.SH NAME`, `.SH SYNOPSIS`, `.SH DESCRIPTION`, and `.SH AUTHOR` section. They were checked locally first with `man -l man/man3/mystrlen.3`, before installing them anywhere.

The `install` target (added to both the root `Makefile` and `src/Makefile`) copies `bin/client_static` to `/usr/local/bin/client`, and copies all the man pages to `/usr/local/share/man/man1` and `/usr/local/share/man/man3`. We installed `client_static` instead of `client_dynamic`, since it doesn't depend on any other file to run — installing the dynamic version system-wide would also mean installing `libmyutils.so` somewhere the system can find it, which the assignment doesn't ask for. After running `sudo make install`, typing `client` works from any folder, and `man client` / `man mystrlen` both work too, since `/usr/local/bin` and `/usr/local/share/man` are already checked by default.

---

## Part 6: Final Notes

All feature branches were finished, tagged, and merged into `main` one after another:

| Branch | Tag | Release |
|---|---|---|
| `multifile-build` | `v0.1.1-multifile` | Version 0.1.0: Multi-file Build |
| `static-build` | `v0.2.1-static` | Version 0.2.1: Static Library Build |
| `dynamic-build` | `v0.3.1-dynamic` | Version 0.3.1: Dynamic Library Build |
| `man-pages` | `v0.4.1-final` | Version 0.4.1: Final Build |

Each branch was merged into `main` before starting the next one, and every branch is still pushed to GitHub so the full history of how the project was built stays visible.

Building this step by step — first compiling everything directly, then a static library, then a dynamic library, then adding documentation and an install step — made the differences between these approaches easy to actually see instead of just reading about them. The same six functions went from being copied straight into one program, to being archived but still copied in at link time, to finally being kept in a separate file and loaded only when the program starts. Checking each stage with `nm`, `readelf`, `ldd`, and `ls -lh` turned "static vs dynamic linking" from a theory topic into something we could actually watch happen.
