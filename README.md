# OpenMPI-filter

## Dependencies

After configuring a NFS shared folder you need to install OpenMPI. And configure the ssh authorized_key file for the master node to be able to execute the program in other nodes. After that, you need to add the corresponding ip of the slave nodes to the `/etc/hosts/` file from the master node.

    sudo apt install openmpi-bin  openmpi-common openmpi-doc libopenmpi-dev

## Compile

From root folder compile using

    make

## Run

Move ./bin/filter to the NFS shared folder with the image to be filtered and execute the command:

    mpirun --hostfile /etc/hosts -np <total_nodes> ./filter <input_image> <output_image> <window_size>


## Measurements

To measure the bandwith you can use NLoad

    sudo apt install nload
    nload <node_ip>

To measure latency you can use ping

    ping <node_ip>
