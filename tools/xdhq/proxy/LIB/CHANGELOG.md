# *XDHq* *CHANGELOG*

## 2022-06-24
- fixing bug introduced with *FaaS* protocol v1,

## 2022-06-03
- adaptation to *FreeBSD*;

## 2022-05-18
- adaptation to changes in underlying modules,
- introducing new *FaaS* protocol (v1): the user content of the *HTML* *head* section is retrieved from the backend as needed and no more cached,

## 2022-04-24
- *xdhqxdh*: using shorter random generated tokens,

## 2022-01-01

- the primitives handling is moved upstream (*xdwhebq*),
- adaptation to *FaaSq*,

## 2021-12-10
- *xdhqxdh*:
  - fixing bug callback deleted while being used,
  - new protocols (both main and *FaaS*), the main protocol being handchecked once and not on each new session,
  - adding scripts version to handle compatibility,

## 2020-10-01

- retrieving HTML tag's default action handling from *XDHWebQ*,
- implementing HTML tag's default action handling retrieved from the registry,

## 2020-06-29

- fixing issue with the *arora* web browser which does not return an answer on the layout related primitives dealing with *XSL*, hence blocking all other clients (the outdated *arora* browser **still** can not handle the *XSL* related layout primitive but does no more, at least, block other clients),

## 2020-02-08

- changing both protocol **labels** (no backward compatibility),
- functions are no more hard-coded, but retrieved from registry,

## 2019-07-06

- adding new layout related functionalities,

## 2019-06-01

- adding CSS rules related functionnalities,

## 2019-03-26

- back-end disconnection does no more generate an exception,
- fixing use of same token bug,

## 2019-03-20

- fixing bug due to use of simple crate/bunch instead of list ones,

## 2019-03-04

- fixing *alert* primitive blocking issue,

## 2019-02-05

- fixing *clang* issue,

## 2018-12-18

- adding token handling through plugin,
- handling error message when problem on connection (added to handle error on token handling),

## 2018-12-07

- switching to new protocol, to allow handling of error messages in future evolution,
- reseting protocol version,

## 2018-11-20

- adding logging,
- switching to protocol version 1,

## 2018-09-09

- improving previous bug fix,

## 2018-09-08

- fixing blocking bug (CPU to 100% under Linux) occurring when the user's app did not establish a new connection,

## 2018-07-17

- handling DEMO mode,

## 2018-02-19:

- the initial action (on new session) is no more automatically launched on mono user mode (desktop app.), but have to be explicitly launched,

## 2018-02-03:
 - adding `Execute`, `Focus`, `RemoveAttribute`.
