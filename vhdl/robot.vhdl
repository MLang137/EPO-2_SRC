--	2022 EPO-2 Smart Robot Challenge
--	Mentor group B4-2

library IEEE;
use IEEE.std_logic_1164.all;

entity robot is
	port (
		clk         		: in    std_logic;
		reset           	: in    std_logic;

		sensor				: in    std_logic_vector(2 downto 0);

		--route_straight		: in	std_logic; -- 1 to go straight
		--route_corner		: in	std_logic; -- 1 to turn left, 0 to turn right
		mine_input					: in	std_logic;
		
		rx						: in std_logic;
		tx						: out std_logic;

		motor_l_pwm     	: out   std_logic;
		motor_r_pwm     	: out   std_logic
	);
end entity robot;

architecture structural of robot is
	
	component timebase is
		port (
			clk				: in	std_logic;
			reset			: in	std_logic;

			count_out		: out	std_logic_vector (19 downto 0)
		);
	end component timebase;

	component inputbuffer is
        port (
			clk				: in	std_logic;
    
            sensor_l_in		: in	std_logic;
            sensor_m_in		: in	std_logic;
            sensor_r_in		: in	std_logic;
    
            sensor_l_out	: out	std_logic;
            sensor_m_out	: out	std_logic;
            sensor_r_out	: out	std_logic
        );
    end component inputbuffer;

    component controller is
        port (
			clk				: in	std_logic;
            reset			: in	std_logic;
    
            sensor_l		: in	std_logic;
            sensor_m		: in	std_logic;
            sensor_r		: in	std_logic;

				route_straight	: in	std_logic;
				route_corner	: in	std_logic;
				mine_input				: in	std_logic;
				
				maneuver_complete: out	std_logic;
				
				uart_read		: out std_logic;
				uart_write		: out std_logic;
    
            count_in		: in	std_logic_vector (19 downto 0);
            count_reset		: out	std_logic;
    
            motor_l_reset		: out	std_logic;
            motor_l_direction	: out	std_logic;
    
            motor_r_reset		: out	std_logic;
            motor_r_direction	: out	std_logic
        );
    end component controller;

	component motorcontrol is
		port (
			clk				: in	std_logic;
			reset			: in	std_logic;
			direction		: in	std_logic;
			count_in		: in	std_logic_vector (19 downto 0);

			pwm				: out	std_logic
		);
	end component motorcontrol;
	
	component uart is
	    port (
        clk             : in  std_logic;
        reset           : in  std_logic;

        rx              : in  std_logic;
        tx              : out std_logic;

        data_in         : in  std_logic_vector (7 downto 0);
        buffer_empty    : out std_logic;
        write           : in  std_logic;

        data_out        : out std_logic_vector (7 downto 0);
        data_ready      : out std_logic;
        read            : in  std_logic
    );
	end component uart;
	
	component eight_bit_ram is
		port (
			ram_data_in			: in std_logic_vector(7 downto 0); -- Data to write into RAM
			ram_write				: in std_logic; -- Write enable 
			clk			: in std_logic; -- clock input for RAM
			reset					: in std_logic;
			ram_data_out		: out std_logic_vector(7 downto 0) -- Data output of RAM
	);
	end component eight_bit_ram;

	-- Global signal
	signal count: std_logic_vector(19 downto 0);
	-- Buffer to controller
	signal buf_sensor_l_cont, buf_sensor_m_cont, buf_sensor_r_cont: std_logic;
	-- Controller to motorcontrollers
	signal cont_direction_mcl, cont_reset_mcl, cont_direction_mcr, cont_reset_mcr: std_logic;
	-- Controller to timebase
	signal cont_reset_tib: std_logic;
	-- Uart unused signals
	signal uart_rx, uart_tx, uart_buffer_empty, uart_data_ready, uart_write, uart_read: std_logic;
	-- Uart to controller
	signal uart_data_out_cont: std_logic_vector(7 downto 0);
	-- Controller to UART
	signal cont_data_in_uart: std_logic_vector(7 downto 0);

begin
	
	
	tib: timebase port map	(
		clk				=> clk,
		reset			=> cont_reset_tib,
		count_out		=> count
	);

	buf: inputbuffer port map (
        clk     		=>  clk,
        sensor_l_in   	=>  sensor(0),
        sensor_m_in   	=>  sensor(1),
        sensor_r_in   	=>  sensor(2),
        sensor_l_out  	=>  buf_sensor_l_cont,
        sensor_m_out   	=>  buf_sensor_m_cont,
        sensor_r_out   	=>  buf_sensor_r_cont
    );
		
	cont: controller port map (
		clk             => clk,
		reset			=> reset,

		sensor_l		=> buf_sensor_l_cont,
		sensor_m		=> buf_sensor_m_cont,
		sensor_r		=> buf_sensor_r_cont,

		route_straight	=> uart_data_out_cont(3),
		route_corner	=> uart_data_out_cont(2),
		
		mine_input				=> mine_input,
		maneuver_complete => cont_data_in_uart(6),
		--mine_output	=> cont_data_in_uart(7),

		uart_write			=> uart_write,
		uart_read			=> uart_read,

		count_in		=> count,
		count_reset		=> cont_reset_tib,

		motor_l_direction	=> cont_direction_mcl,
		motor_l_reset		=> cont_reset_mcl,

		motor_r_direction	=> cont_direction_mcr,
		motor_r_reset		=> cont_reset_mcr
	);

	mcl: motorcontrol port map (
		clk				=> clk,
		reset			=> cont_reset_mcl,
		direction		=> cont_direction_mcl,
		count_in		=> count,
		pwm				=> motor_l_pwm
	);

	mcr: motorcontrol port map (
		clk				=> clk,
		reset			=> cont_reset_mcr,
		direction		=> cont_direction_mcr,
		count_in		=> count,
		pwm				=> motor_r_pwm
	);
	
	urt: uart port map (
        clk				=> clk,
        reset           => reset,

        rx              => rx,
        tx              => tx,

        data_in         => cont_data_in_uart,
        buffer_empty    => uart_buffer_empty,
        write           => uart_write,

        data_ready      => uart_data_ready,
        read            => uart_read,
		  data_out        => uart_data_out_cont
	);
	
	cont_data_in_uart(5) <= '1';
	cont_data_in_uart(4) <= '0';
	cont_data_in_uart(3) <= '1';
	cont_data_in_uart(2) <= '1';
	cont_data_in_uart(1) <= '1';
	cont_data_in_uart(0) <= '1';

end architecture structural;