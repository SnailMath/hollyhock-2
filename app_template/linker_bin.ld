ENTRY(_main);

SECTIONS {
	start_address = 0x8CFF0000;
	.init start_address : AT(start_address) {
		*(.init)
	}
	info_address = 0x8CFF0010;
	. = info_address;
	.hollyhock_name : {
		*(.hollyhock_name)
	}
	.hollyhock_description : {
		*(.hollyhock_description)
	}
	.hollyhock_author : {
		*(.hollyhock_author)
	}
	.hollyhock_version : {
		*(.hollyhock_version)
	}
	.text : {
		*(.text)
	}
}



