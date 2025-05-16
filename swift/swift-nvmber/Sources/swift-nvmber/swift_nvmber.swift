// The Swift Programming Language
// https://docs.swift.org/swift-book
public class Nvmber {
	var chars = String()
	var integer = 0

	init(fromStr str: any StringProtocol) {
		chars = str as! String
		integer = parse(str: str)!
	}

	func parse(str: any StringProtocol) -> Int? {
		var integer: Int?
		switch str as! String {
			case "X":
				integer = 10
			case "I":
				integer = 0
			default:
				integer = nil
		}
		return integer
	}
}
