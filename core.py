from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.private_l1_shared_l2_cache_hierarchy import PrivateL1SharedL2CacheHierarchy
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import BinaryResource
from gem5.simulate.simulator import Simulator

# Define custom L1 data cache with associativity
cache_hierarchy = PrivateL1SharedL2CacheHierarchy(
    l1d_size="32kB",
    l1d_assoc=8,
    l1i_size="32kB",
    l1i_assoc=8,
    l2_size="3MB",
    l2_assoc=12,
)

memory = SingleChannelDDR3_1600("4GiB")

#  Atomic CPU (Functional)
#cpu_type = CPUTypes.ATOMIC

# cpu_type = CPUTypes.TIMING

# Note: O3CPU is the only CPU here that is modeled off of a real CPU
# Uncomment and look at this cpu_type at home for fun!
cpu_type = CPUTypes.O3

processor = SimpleProcessor(cpu_type=cpu_type, isa=ISA.X86, num_cores=16)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

binary = BinaryResource(local_path="/mnt/c/Users/moath/Desktop/study/gem5/fs/data_race.exe")
board.set_se_binary_workload(binary, arguments=["-t","w","-n", "2", "-a", "s", "-s", "n"])

simulator = Simulator(board=board)
simulator.run()
