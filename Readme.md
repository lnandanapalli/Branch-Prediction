### Environment

-   **Platform:** Windows + Cygwin POSIX layer
-   **Required Cygwin packages**

    | Package    | Purpose                            |
    | ---------- | ---------------------------------- |
    | `tcsh`     | C-shell interpreter for run script |
    | `bc`       | Calculator utility used in scripts |
    | `gcc-core` | C compiler                         |
    | `gcc-g++`  | C++ compiler                       |
    | `make`     | Build automation                   |

Standard Cygwin tools cover the rest.

---

### Build & Run

1. **Open a Cygwin terminal.**

2. **Go to the `infrastructure` directory**

    cd /cygdrive/c/path/to/project/infrastructure

3. **Compile the sources**

    cd src # enter the source tree
    make # build everything

4. **Run the branch-prediction experiment**

    cd .. # back to infrastructure root
    csh run traces

The branch-prediction results print directly to the terminal.
