--	2022 EE1D21 Line-follower
--	Mentor group B4, students:
--	Matthijs Langenberg		5557313
--	Maarten Oudijk			5595533


library IEEE;
use IEEE.std_logic_1164.all;

-- Register
entity three_bit_reg is
	port (
		clk	: in std_logic;
		in1:	in std_logic;
		in2:	in std_logic;
		in3:	in std_logic;
		out1:	out std_logic;
		out2:	out std_logic;
		out3:	out std_logic
	);
end entity three_bit_reg;

architecture behavioural of three_bit_reg is
	
begin
	
	reg: process(clk)
	
	begin
		if rising_edge(clk) then
			out1 <= in1;
			out2 <= in2;
			out3 <= in3;
		end if;
	end process;
	
end architecture behavioural;



library IEEE;
use IEEE.std_logic_1164.all;

-- Inputbuffer
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
	component three_bit_reg is
		port (
			clk	: in std_logic;
			in1:	in std_logic;
			in2:	in std_logic;
			in3:	in std_logic;
			out1:	out std_logic;
			out2:	out std_logic;
			out3:	out std_logic
		);
	end component three_bit_reg;

	signal s1, s2, s3: std_logic;

begin
	
	R1:    three_bit_reg port map (
        clk     =>  clk,
        in1   =>  sensor_l_in,
        in2   =>  sensor_m_in,
        in3   =>  sensor_r_in,
        out1   =>  s1,
        out2   =>  s2,
        out3   =>  s3
    );

	R2:    three_bit_reg port map (
        clk     =>  clk,
        in1   =>  s1,
        in2   =>  s2,
        in3   =>  s3,
        out1   =>  sensor_l_out,
        out2   =>  sensor_m_out,
        out3   =>  sensor_r_out
    );
	
end architecture structural;