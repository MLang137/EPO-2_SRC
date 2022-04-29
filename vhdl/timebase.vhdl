library IEEE;

use IEEE.std_logic_1164.all;

use IEEE.numeric_std.all;

 

entity timebase is

                port (  clk                             : in std_logic;

                                reset                     : in std_logic;

                              ---  enable                  : in std_logic;

                                count_out           : out std_logic_vector (19 downto 0)

                );

end entity timebase;

 

architecture behavioural of timebase is

 

                signal     count, new_count           : unsigned (19 downto 0);

 

begin

                --- This process generates the register

                process (clk)

                begin

                                if (rising_edge (clk)) then

                                                if (reset = '1') then

                                                                count <= (others => '0'); --- set a 0 to reset

                                                else

                                                                count <= new_count;

                                                end if;

                                end if;

                end process;

 

                --- This process calculates the new count-value

                process (count)

                begin

                              ---  if (enable = '1') then

                                           new_count         <= count + 1;                      --- only apply addition if enable = '1'

                              --- else

                               ---                 new_count         <= count;

                              ---  end if;

                end process;

 

                --- With this line, the count-value is brought to the outside

                count_out           <= std_logic_vector (count);

 

end architecture behavioural;