Import("env")

# GNU ar s creates the same archive symbol index as ranlib.
# Use the already selected archiver because Windows blocks gcc-ranlib.exe.
env.Replace(RANLIBCOM="$AR s $TARGET")
