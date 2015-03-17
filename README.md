Copyright (C) 2014-2015 Ernold Vasiliev <ericv@mail.ru>

    SSN project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSN project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSN project.  If not, see <http://www.gnu.org/licenses/>.

# ssn
command line SSN telemetry data requester. 
Request preferences in command argument1 in JSON format:

	{"ssn":{
	"json_file_name":"getdevvals.json",
	"com_port_name":"/dev/ttyUSB0",
	"com_port_speed":57600,
	"obj_dst":1,
	"obj_src":3
	}}


##	./ssn <json_string>

##example using in scripts folder of main SSN project: "dataload.sh"
