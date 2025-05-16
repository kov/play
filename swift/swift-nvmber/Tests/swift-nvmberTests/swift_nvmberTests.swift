import XCTest
@testable import swift_nvmber

final class swift_nvmberTests: XCTestCase {
    func testExample() throws {
        let nvmber = Nvmber(fromStr: "X")
        assert(nvmber.integer == 10)
    }
}
