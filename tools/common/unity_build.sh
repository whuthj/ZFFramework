#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
TARGET_FILE=$1
SRC_DIRS=( "$@" )
SRC_DIR_COUNT=${#SRC_DIRS[@]}

if test "x$TARGET_FILE" = "x" || test "x$SRC_DIR_COUNT" = "x0" ; then
    echo usage:
    echo   unity_build.sh TARGET_FILE [SRC_DIRS, ...]
    exit 1
fi

rm "${TARGET_FILE}.tmp" >/dev/null 2>&1
mkdir -p "${TARGET_FILE}.tmp" >/dev/null 2>&1
rm -rf "${TARGET_FILE}.tmp" >/dev/null 2>&1

for (( i = 1; i < ${SRC_DIR_COUNT}; ++i));
do
    find "${SRC_DIRS[$i]}" -name "*.c" >> "${TARGET_FILE}.tmp"
    find "${SRC_DIRS[$i]}" -name "*.cxx" >> "${TARGET_FILE}.tmp"
    find "${SRC_DIRS[$i]}" -name "*.cpp" >> "${TARGET_FILE}.tmp"
    find "${SRC_DIRS[$i]}" -name "*.m" >> "${TARGET_FILE}.tmp"
    find "${SRC_DIRS[$i]}" -name "*.mm" >> "${TARGET_FILE}.tmp"
done

sed -i '' 's/^\(.*\)$/#include "\1"/g' "${TARGET_FILE}.tmp"


need_gen="0"

if test ! -e "${TARGET_FILE}"; then
    need_gen="1"
else
    md5_src=`md5 "${TARGET_FILE}.tmp" | sed 's/^.* = \(.*\)$/\1/g'`
    md5_dst=`md5 "${TARGET_FILE}" | sed 's/^.* = \(.*\)$/\1/g'`
    if [ "$md5_src" != "$md5_dst" ]; then
        need_gen="1"
    fi
fi

if [ ${need_gen} = "1" ]; then
    mv "${TARGET_FILE}.tmp" "${TARGET_FILE}"
else
    rm "${TARGET_FILE}.tmp" >/dev/null 2>&1
fi

