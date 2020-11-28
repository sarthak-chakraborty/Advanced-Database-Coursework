const contract1 = artifacts.require("SGPA");

module.exports = function (deployer) {
  deployer.deploy(contract1);
};