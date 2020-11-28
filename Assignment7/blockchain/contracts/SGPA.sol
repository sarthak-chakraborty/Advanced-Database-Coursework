pragma solidity >=0.4.22;

contract SGPA {
    struct Course{
        uint id;
        uint credit;
        string grade;
        uint grade_point;
    }

    Course[] course;

    uint tot_grade_point = 0;
    uint tot_credit = 0;

    uint sgpa;


    function setCourse(uint _id, uint _credit, string memory _grade) public {
        // course.length++;

        Course memory c;

        c.id = _id;
        c.credit = _credit;
        c.grade = _grade;
        
        if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("E"))){
            c.grade_point = 10;
        } else if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("A"))){
            c.grade_point = 9;
        } else if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("B"))){
            c.grade_point = 8;
        } else if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("C"))){
            c.grade_point = 7;
        } else if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("D"))){
            c.grade_point = 6;
        } else if(keccak256(abi.encodePacked(_grade)) == keccak256(abi.encodePacked("P"))){
            c.grade_point = 5;
        } else {
            c.grade_point = 0;
        }
        course.push(c);
    }


    function getCourseID(uint _i) public view returns (uint) {
        return course[_i].id;
    }
    function getCredit(uint _i) public view returns(uint) {
        return course[_i].credit;
    }
    function getGrade(uint _i) public view returns(string memory) {
        return course[_i].grade;
    }
    function getGradePoint(uint _i) public view returns(uint) {
        return course[_i].grade_point;
    }
    
    function calcSGPA() public {
        for(uint i=0; i < course.length; i++){
            tot_grade_point = tot_grade_point + course[i].grade_point * course[i].credit;
            tot_credit = tot_credit + course[i].credit;
        }

        sgpa = (tot_grade_point * 100) / tot_credit;
    }  

    function getSGPA() public view returns(uint) {
        return sgpa;
    }  

    function getGradePoint() public view returns(uint) {
        return tot_grade_point;
    }
}