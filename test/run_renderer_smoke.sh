#!/usr/bin/env bash

set -u

renderer_smoke="$1"

# Xvfb occasionally wins its own readiness race on busy CI/dev machines. A
# renderer failure must reproduce; a transient display allocation failure may
# retry on a fresh automatic display number.
for attempt in 1 2 3; do
    if xvfb-run -a -e /dev/stderr \
            --server-args="-screen 0 1280x1024x24" \
            "$renderer_smoke"; then
        exit 0
    fi

    if [ "$attempt" -lt 3 ]; then
        sleep 0.2
    fi
done

exit 1
