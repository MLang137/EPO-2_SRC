library IEEE;
use IEEE.std_logic_1164.all;
entity d_ff is 
port(d, clk : in std_logic; q : out std_logic);
end d_ff;

architecture behav_dff of d_ff is
begin
ff_behavior : process is 
	begin 
	wait until(clk'event and clk='1');
	q <= d;
end process;
end behav_dff;


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
entity inputbuffer is
	port (	clk		: in	std_logic;

		sensor_l_in	: in	std_logic;
		sensor_m_in	: in	std_logic;
		sensor_r_in	: in	std_logic;

		sensor_l_out	: out	std_logic;
		sensor_m_out	: out	std_logic;
		sensor_r_out	: out	std_logic
	);
end entity inputbuffer;


architecture structural of inputbuffer is
component d_ff is
      port (d, clk : in std_logic; q : out std_logic);
   end component d_ff;
signal I,M,R: std_logic;
begin
   ff0: d_ff port map (d => sensor_l_in, clk => clk, q => I);
   ff1: d_ff port map (d => sensor_m_in, clk => clk, q => M);   
   ff2: d_ff port map (d => sensor_r_in, clk => clk, q => R);

   ff3: d_ff port map (d => I, clk => clk, q => sensor_l_out);
   ff4: d_ff port map (d => M, clk => clk, q => sensor_m_out);   
   ff5: d_ff port map (d => R, clk => clk, q => sensor_r_out);
end architecture structural;

