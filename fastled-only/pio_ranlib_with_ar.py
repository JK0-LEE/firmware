Import("env")

# On this Windows setup, use ar to update the archive index when ranlib is blocked.
env.Replace(RANLIBCOM="$AR s $TARGET")
