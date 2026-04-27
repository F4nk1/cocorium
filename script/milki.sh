#!/bin/bash

OUTPUT=$COCORIUM/build/milki.sql

cat > $OUTPUT << 'EOF'

-- Disable temporal restrictions


cat >> $OUTPUT << 'EOF'

-- Enable restrictions
SET session_replication_role = DEFAULT;
OEF

# run en postresql

psql -U postgres -d milki -f $OUTPUT