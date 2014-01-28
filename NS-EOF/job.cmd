#!/bin/bash
#
#@ job_type = parallel
#@ class = general
#@ node = 4
#@ tasks_per_node = 16
#@ wall_clock_limit = 1:20:30
#@ job_name = mytest
#@ network.MPI = sn_all,not_shared,us
#@ initialdir = $(home)/mydir
#@ output = job$(jobid).out
#@ error = job$(jobid).err
#@ notification=always
#@ notify_user=ga46laz@mytum.de
#@ queue
# LOAD module environment
. /etc/profile
. /etc/profile.d/modules.sh
module add lrztools
# set POE environment
export MP_NEWJOB=parallel
export MP_LABELIO=yes
cat << EOF > llcmdfile.cmd
./subjob.ibm@1%64%mpich2:*
./subjob.ibm@2%32%mpich2:*
./subjob.ibm@3%32%mpich2:*
complete
EOF
# EXECUTE CMD job
poe -cmdfile llcmdfile.cmd
# Optional: SPLIT ouput for single SUBJOBS
# uses LOADL_STEP_ERR and LOAD_STEP_OUT
get_cmd_out -cmdfile llcmdfile.cmd   
