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

_TODO_

### Q2. What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?

_TODO_

### Q3. When you run `nm` on `client_static`, are the symbols for functions like `mystrlen` present? What does this tell you about static linking?

_TODO_

---

## Part 4: Dynamic Library

### Q1. What is Position-Independent Code (`-fPIC`) and why is it required for shared libraries?

_TODO_

### Q2. Explain the difference in file size between the static and dynamic clients. Why does this difference exist?

_TODO_

### Q3. What is `LD_LIBRARY_PATH`? Why was it necessary, and what does this tell you about the responsibilities of the dynamic loader?

_TODO_

---

## Part 5: Man Pages & Installation

_(No report questions specified for this part in the assignment; notes on the install process can go here.)_

---

## Part 6: Final Notes

_TODO — summary once all branches are merged and pushed._
