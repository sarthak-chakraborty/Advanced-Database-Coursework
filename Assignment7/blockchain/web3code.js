const Web3 = require('web3');
const { Contract } = require('web3-eth-contract');
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));
const Tx = require('ethereumjs-tx').Transaction


let addr1 = '0xbCa0994CcC9fE25C727c16B5C818eda99C536172'
let privateKey1 = Buffer.from('d9ae9f78176b9027c268aacd949438c5d3a4fb6a34faeba43dd2cb09d9468fa4', 'hex');

let contractAddr = "0x2255Bb06020E2298Fa8FEF2820eAc01Fc484de45"
let contractABI = [
  {
    "constant": false,
    "inputs": [
      {
        "internalType": "uint256",
        "name": "_id",
        "type": "uint256"
      },
      {
        "internalType": "uint256",
        "name": "_credit",
        "type": "uint256"
      },
      {
        "internalType": "string",
        "name": "_grade",
        "type": "string"
      }
    ],
    "name": "setCourse",
    "outputs": [],
    "payable": false,
    "stateMutability": "nonpayable",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [
      {
        "internalType": "uint256",
        "name": "_i",
        "type": "uint256"
      }
    ],
    "name": "getCourseID",
    "outputs": [
      {
        "internalType": "uint256",
        "name": "",
        "type": "uint256"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [
      {
        "internalType": "uint256",
        "name": "_i",
        "type": "uint256"
      }
    ],
    "name": "getCredit",
    "outputs": [
      {
        "internalType": "uint256",
        "name": "",
        "type": "uint256"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [
      {
        "internalType": "uint256",
        "name": "_i",
        "type": "uint256"
      }
    ],
    "name": "getGrade",
    "outputs": [
      {
        "internalType": "string",
        "name": "",
        "type": "string"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  },
  {
    "constant": false,
    "inputs": [],
    "name": "calcSGPA",
    "outputs": [],
    "payable": false,
    "stateMutability": "nonpayable",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [],
    "name": "getSGPA",
    "outputs": [
      {
        "internalType": "uint256",
        "name": "",
        "type": "uint256"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [
      {
        "internalType": "uint256",
        "name": "_i",
        "type": "uint256"
      }
    ],
    "name": "getGradePoint",
    "outputs": [
      {
        "internalType": "uint256",
        "name": "",
        "type": "uint256"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  },
  {
    "constant": true,
    "inputs": [],
    "name": "getGradePoint",
    "outputs": [
      {
        "internalType": "uint256",
        "name": "",
        "type": "uint256"
      }
    ],
    "payable": false,
    "stateMutability": "view",
    "type": "function"
  }
]

let contract1 = new web3.eth.Contract(contractABI, contractAddr); 


function sendTx(_nonce, _to, _data) {
    let txObject = {
        nonce : web3.utils.toHex(_nonce),
        gasLimit : web3.utils.toHex(800000),
        gasPrice : web3.utils.toHex(web3.utils.toWei('20', 'gwei')),
        to : _to,
        data : _data.encodeABI()
    }

    let tx = new Tx(txObject);
    tx.sign(privateKey1);

    let serializedTx = tx.serialize();
    let raw = '0x' + serializedTx.toString('hex');

    web3.eth.sendSignedTransaction(raw, (err, txHash) => {
        console.log('error :', err);
        console.log('txHash :', txHash);
    });
}

// web3.eth.getTransactionCount("0x0Ee7473a1a492054D1fF1D8395BfBcc69668a82f").then(console.log);

sendTx(0, contractAddr, contract1.methods.setCourse(60003, 4, "A"));
sendTx(1, contractAddr, contract1.methods.setCourse(43002, 5, "E"));
sendTx(2, contractAddr, contract1.methods.setCourse(60010, 3, "E"));
sendTx(3, contractAddr, contract1.methods.setCourse(41082, 3, "E"));
sendTx(4, contractAddr, contract1.methods.setCourse(47006, 6, "E"));

sendTx(5, contractAddr, contract1.methods.calcSGPA());


contract1.methods.getSGPA().call((err, res) => {
    console.log(res);
})


